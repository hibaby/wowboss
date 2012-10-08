
#include "SmallObj.h"
#include <cassert>
#include <climits>
#include <vector>
#include <bitset>

class Chunk
{
private:
    friend class FixedAllocator;

    bool Init( std::size_t blockSize, unsigned char blocks );
    
	void * Allocate( std::size_t blockSize );
    void Deallocate( void * p, std::size_t blockSize );

    void Reset( std::size_t blockSize, unsigned char blocks );
	
	void Release();

    bool IsCorrupt( unsigned char numBlocks, std::size_t blockSize,
        bool checkIndexes ) const;

	bool IsBlockAvailable( void * p, unsigned char numBlocks,
        std::size_t blockSize ) const;

    inline bool HasBlock( void * p, std::size_t chunkLength ) const
    {
        unsigned char * pc = static_cast< unsigned char * >( p );
        return ( pData_ <= pc ) && ( pc < pData_ + chunkLength );
    }

    inline bool HasAvailable( unsigned char numBlocks ) const
    { return ( blocksAvailable_ == numBlocks ); }

    inline bool IsFilled( void ) const
    { return ( 0 == blocksAvailable_ ); }

    /// Pointer to array of allocated blocks.
    unsigned char * pData_;
    /// Index of first empty block.
    unsigned char firstAvailableBlock_;
    /// Count of empty blocks.
    unsigned char blocksAvailable_;
};


class FixedAllocator
{
private:
    void DoDeallocate( void * p );
    bool MakeNewChunk( void );
    Chunk * VicinityFind( void * p ) const;

    /// Not implemented.
    FixedAllocator(const FixedAllocator&);
    /// Not implemented.
    FixedAllocator& operator=(const FixedAllocator&);

    typedef std::vector< Chunk > Chunks;
    typedef Chunks::iterator ChunkIter;
    typedef Chunks::const_iterator ChunkCIter;

    /// Fewest # of objects managed by a Chunk.
    static unsigned char MinObjectsPerChunk_;

    /// Most # of objects managed by a Chunk - never exceeds UCHAR_MAX.
    static unsigned char MaxObjectsPerChunk_;

    std::size_t blockSize_;
    unsigned char numBlocks_;

    Chunks chunks_;
    
	Chunk * allocChunk_;
    Chunk * deallocChunk_;
    /// Pointer to the only empty Chunk if there is one, else NULL.
    Chunk * emptyChunk_;

public:
    FixedAllocator();
	~FixedAllocator();

    void Initialize( std::size_t blockSize, std::size_t pageSize );

    /** Returns pointer to allocated memory block of fixed size - or NULL
        if it failed to allocate.
        */
    void * Allocate( void );

    /** Deallocate a memory block previously allocated with Allocate.  If
        the block is not owned by this FixedAllocator, it returns false so
        that SmallObjAllocator can call the default deallocator.  If the
        block was found, this returns true.
        */
    bool Deallocate( void * p, Chunk * hint );

    /// Returns block size with which the FixedAllocator was initialized.
    inline std::size_t BlockSize() const { return blockSize_; }

    /** Releases the memory used by the empty Chunk.  This will take
        constant time under any situation.
        @return True if empty chunk found and released, false if none empty.
        */
    bool TrimEmptyChunk( void );

    /** Releases unused spots from ChunkList.  This takes constant time
        with respect to # of Chunks, but actual time depends on underlying
        memory allocator.
        @return False if no unused spots, true if some found and released.
        */
    bool TrimChunkList( void );

    /** Returns count of empty Chunks held by this allocator.  Complexity
        is O(C) where C is the total number of Chunks - empty or used.
        */
    std::size_t CountEmptyChunks( void ) const;

    /** Determines if FixedAllocator is corrupt.  Checks data members to
        see if any have erroneous values, or violate class invariants.  It
        also checks if any Chunk is corrupt.  Complexity is O(C) where C is
        the number of Chunks.  If any data is corrupt, this will return true
        in release mode, or assert in debug mode.
        */
    bool IsCorrupt( void ) const;

    /** Returns true if the block at address p is within a Chunk owned by
        this FixedAllocator.  Complexity is O(C) where C is the total number
        of Chunks - empty or used.
        */
    const Chunk * HasBlock( void * p ) const;
    inline Chunk * HasBlock( void * p )
    {
        return const_cast< Chunk * >(
            const_cast< const FixedAllocator * >( this )->HasBlock( p ) );
    }

};

unsigned char FixedAllocator::MinObjectsPerChunk_ = 8;
unsigned char FixedAllocator::MaxObjectsPerChunk_ = UCHAR_MAX;

// Chunk::Init ----------------------------------------------------------------

bool Chunk::Init( std::size_t blockSize, unsigned char blocks )
{
    assert(blockSize > 0);
    assert(blocks > 0);
    // Overflow check
    const std::size_t allocSize = blockSize * blocks;
    assert( allocSize / blockSize == blocks);

#ifdef USE_NEW_TO_ALLOCATE
    // If this new operator fails, it will throw, and the exception will get
    // caught one layer up.
    pData_ = static_cast< unsigned char * >( ::operator new ( allocSize ) );
#else
    // malloc can't throw, so its only way to indicate an error is to return
    // a NULL pointer, so we have to check for that.
    pData_ = static_cast< unsigned char * >( ::std::malloc( allocSize ) );
    if ( NULL == pData_ ) return false;
#endif

    Reset( blockSize, blocks );
    return true;
}

// Chunk::Reset ---------------------------------------------------------------

void Chunk::Reset(std::size_t blockSize, unsigned char blocks)
{
    assert(blockSize > 0);
    assert(blocks > 0);
    // Overflow check
    assert((blockSize * blocks) / blockSize == blocks);

    firstAvailableBlock_ = 0;
    blocksAvailable_ = blocks;

    unsigned char i = 0;
    for ( unsigned char * p = pData_; i != blocks; p += blockSize )
    {
        *p = ++i;
    }
}

// Chunk::Release -------------------------------------------------------------

void Chunk::Release()
{
    assert( NULL != pData_ );
#ifdef USE_NEW_TO_ALLOCATE
    ::operator delete ( pData_ );
#else
    ::std::free( static_cast< void * >( pData_ ) );
#endif
}

// Chunk::Allocate ------------------------------------------------------------

void* Chunk::Allocate(std::size_t blockSize)
{
    if ( IsFilled() ) return NULL;

    assert((firstAvailableBlock_ * blockSize) / blockSize == 
        firstAvailableBlock_);
    unsigned char * pResult = pData_ + (firstAvailableBlock_ * blockSize);
    firstAvailableBlock_ = *pResult;
    --blocksAvailable_;

    return pResult;
}

// Chunk::Deallocate ----------------------------------------------------------

void Chunk::Deallocate(void* p, std::size_t blockSize)
{
    assert(p >= pData_);

    unsigned char* toRelease = static_cast<unsigned char*>(p);
    // Alignment check
    assert((toRelease - pData_) % blockSize == 0);
    unsigned char index = static_cast< unsigned char >(
        ( toRelease - pData_ ) / blockSize);

#if defined(DEBUG) || defined(_DEBUG)
    // Check if block was already deleted.  Attempting to delete the same
    // block more than once causes Chunk's linked-list of stealth indexes to
    // become corrupt.  And causes count of blocksAvailable_ to be wrong.
    if ( 0 < blocksAvailable_ )
        assert( firstAvailableBlock_ != index );
#endif

    *toRelease = firstAvailableBlock_;
    firstAvailableBlock_ = index;
    // Truncation check
    assert(firstAvailableBlock_ == (toRelease - pData_) / blockSize);

    ++blocksAvailable_;
}

// Chunk::IsCorrupt -----------------------------------------------------------

bool Chunk::IsCorrupt( unsigned char numBlocks, std::size_t blockSize,
    bool checkIndexes ) const
{

    if ( numBlocks < blocksAvailable_ )
    {
        // Contents at this Chunk corrupted.  This might mean something has
        // overwritten memory owned by the Chunks container.
        assert( false );
        return true;
    }
    if ( IsFilled() )
        // Useless to do further corruption checks if all blocks allocated.
        return false;
    unsigned char index = firstAvailableBlock_;
    if ( numBlocks <= index )
    {
        // Contents at this Chunk corrupted.  This might mean something has
        // overwritten memory owned by the Chunks container.
        assert( false );
        return true;
    }
    if ( !checkIndexes )
        // Caller chose to skip more complex corruption tests.
        return false;

    /* If the bit at index was set in foundBlocks, then the stealth index was
     found on the linked-list.
     */
    std::bitset< UCHAR_MAX > foundBlocks;
    unsigned char * nextBlock = NULL;

    /* The loop goes along singly linked-list of stealth indexes and makes sure
     that each index is within bounds (0 <= index < numBlocks) and that the
     index was not already found while traversing the linked-list.  The linked-
     list should have exactly blocksAvailable_ nodes, so the for loop will not
     check more than blocksAvailable_.  This loop can't check inside allocated
     blocks for corruption since such blocks are not within the linked-list.
     Contents of allocated blocks are not changed by Chunk.

     Here are the types of corrupted link-lists which can be verified.  The
     corrupt index is shown with asterisks in each example.

     Type 1: Index is too big.
      numBlocks == 64
      blocksAvailable_ == 7
      firstAvailableBlock_ -> 17 -> 29 -> *101*
      There should be no indexes which are equal to or larger than the total
      number of blocks.  Such an index would refer to a block beyond the
      Chunk's allocated domain.

     Type 2: Index is repeated.
      numBlocks == 64
      blocksAvailable_ == 5
      firstAvailableBlock_ -> 17 -> 29 -> 53 -> *17* -> 29 -> 53 ...
      No index should be repeated within the linked-list since that would
      indicate the presence of a loop in the linked-list.
     */
    for ( unsigned char cc = 0; ; )
    {
        nextBlock = pData_ + ( index * blockSize );
        foundBlocks.set( index, true );
        ++cc;
        if ( cc >= blocksAvailable_ )
            // Successfully counted off number of nodes in linked-list.
            break;
        index = *nextBlock;
        if ( numBlocks <= index )
        {
            /* This catches Type 1 corruptions as shown in above comments.
             This implies that a block was corrupted due to a stray pointer
             or an operation on a nearby block overran the size of the block.
             */
            assert( false );
            return true;
        }
        if ( foundBlocks.test( index ) )
        {
            /* This catches Type 2 corruptions as shown in above comments.
             This implies that a block was corrupted due to a stray pointer
             or an operation on a nearby block overran the size of the block.
             Or perhaps the program tried to delete a block more than once.
             */
            assert( false );
            return true;
        }
    }
    if ( foundBlocks.count() != blocksAvailable_ )
    {
        /* This implies that the singly-linked-list of stealth indexes was
         corrupted.  Ideally, this should have been detected within the loop.
         */
        assert( false );
        return true;
    }

    return false;
}

// Chunk::IsBlockAvailable ----------------------------------------------------

bool Chunk::IsBlockAvailable( void * p, unsigned char numBlocks,
    std::size_t blockSize ) const
{
    (void) numBlocks;
    
    if ( IsFilled() )
        return false;

    unsigned char * place = static_cast< unsigned char * >( p );
    // Alignment check
    assert( ( place - pData_ ) % blockSize == 0 );
    unsigned char blockIndex = static_cast< unsigned char >(
        ( place - pData_ ) / blockSize );

    unsigned char index = firstAvailableBlock_;
    assert( numBlocks > index );
    if ( index == blockIndex )
        return true;

    /* If the bit at index was set in foundBlocks, then the stealth index was
     found on the linked-list.
     */
    std::bitset< UCHAR_MAX > foundBlocks;
    unsigned char * nextBlock = NULL;
    for ( unsigned char cc = 0; ; )
    {
        nextBlock = pData_ + ( index * blockSize );
        foundBlocks.set( index, true );
        ++cc;
        if ( cc >= blocksAvailable_ )
            // Successfully counted off number of nodes in linked-list.
            break;
        index = *nextBlock;
        if ( index == blockIndex )
            return true;
        assert( numBlocks > index );
        assert( !foundBlocks.test( index ) );
    }

    return false;
}

// FixedAllocator::FixedAllocator ---------------------------------------------

FixedAllocator::FixedAllocator()
    : blockSize_( 0 )
    , numBlocks_( 0 )
    , chunks_( 0 )
    , allocChunk_( NULL )
    , deallocChunk_( NULL )
    , emptyChunk_( NULL )
{
}

// FixedAllocator::~FixedAllocator --------------------------------------------

FixedAllocator::~FixedAllocator()
{
#ifdef DO_EXTRA_LOKI_TESTS
    TrimEmptyChunk();
    assert( chunks_.empty() && "Memory leak detected!" );
#endif
    for ( ChunkIter i( chunks_.begin() ); i != chunks_.end(); ++i )
       i->Release();
}

// FixedAllocator::Initialize -------------------------------------------------

void FixedAllocator::Initialize( std::size_t blockSize, std::size_t pageSize )
{
    assert( blockSize > 0 );
    assert( pageSize >= blockSize );
    blockSize_ = blockSize;

    std::size_t numBlocks = pageSize / blockSize;
    if ( numBlocks > MaxObjectsPerChunk_ ) numBlocks = MaxObjectsPerChunk_;
    else if ( numBlocks < MinObjectsPerChunk_ ) numBlocks = MinObjectsPerChunk_;

    numBlocks_ = static_cast<unsigned char>(numBlocks);
    assert(numBlocks_ == numBlocks);
}

// FixedAllocator::CountEmptyChunks -------------------------------------------

std::size_t FixedAllocator::CountEmptyChunks( void ) const
{
#ifdef DO_EXTRA_LOKI_TESTS
    // This code is only used for specialized tests of the allocator.
    // It is #ifdef-ed so that its O(C) complexity does not overwhelm the
    // functions which call it.
    std::size_t count = 0;
    for ( ChunkCIter it( chunks_.begin() ); it != chunks_.end(); ++it )
    {
        const Chunk & chunk = *it;
        if ( chunk.HasAvailable( numBlocks_ ) )
            ++count;
    }
    return count;
#else
    return ( NULL == emptyChunk_ ) ? 0 : 1;
#endif
}

// FixedAllocator::IsCorrupt --------------------------------------------------

bool FixedAllocator::IsCorrupt( void ) const
{
    const bool isEmpty = chunks_.empty();
    ChunkCIter start( chunks_.begin() );
    ChunkCIter last( chunks_.end() );
    const size_t emptyChunkCount = CountEmptyChunks();

    if ( isEmpty )
    {
        if ( start != last )
        {
            assert( false );
            return true;
        }
        if ( 0 < emptyChunkCount )
        {
            assert( false );
            return true;
        }
        if ( NULL != deallocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( NULL != allocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( NULL != emptyChunk_ )
        {
            assert( false );
            return true;
        }
    }

    else
    {
        const Chunk * front = &chunks_.front();
        const Chunk * back  = &chunks_.back();
        if ( start >= last )
        {
            assert( false );
            return true;
        }
        if ( back < deallocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( back < allocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( front > deallocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( front > allocChunk_ )
        {
            assert( false );
            return true;
        }

        switch ( emptyChunkCount )
        {
            case 0:
                if ( emptyChunk_ != NULL )
                {
                    assert( false );
                    return true;
                }
                break;
            case 1:
                if ( emptyChunk_ == NULL )
                {
                    assert( false );
                    return true;
                }
                if ( back < emptyChunk_ )
                {
                    assert( false );
                    return true;
                }
                if ( front > emptyChunk_ )
                {
                    assert( false );
                    return true;
                }
                if ( !emptyChunk_->HasAvailable( numBlocks_ ) )
                {
                    // This may imply somebody tried to delete a block twice.
                    assert( false );
                    return true;
                }
                break;
            default:
                assert( false );
                return true;
        }
        for ( ChunkCIter it( start ); it != last; ++it )
        {
            const Chunk & chunk = *it;
            if ( chunk.IsCorrupt( numBlocks_, blockSize_, true ) )
                return true;
        }
    }

    return false;
}

// FixedAllocator::HasBlock ---------------------------------------------------

const Chunk * FixedAllocator::HasBlock( void * p ) const
{
    const std::size_t chunkLength = numBlocks_ * blockSize_;
    for ( ChunkCIter it( chunks_.begin() ); it != chunks_.end(); ++it )
    {
        const Chunk & chunk = *it;
        if ( chunk.HasBlock( p, chunkLength ) )
            return &chunk;
    }
    return NULL;
}

// FixedAllocator::TrimEmptyChunk ---------------------------------------------

bool FixedAllocator::TrimEmptyChunk( void )
{
    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
    if ( NULL == emptyChunk_ ) return false;

    // If emptyChunk_ points to valid Chunk, then chunk list is not empty.
    assert( !chunks_.empty() );
    // And there should be exactly 1 empty Chunk.
    assert( 1 == CountEmptyChunks() );

    Chunk * lastChunk = &chunks_.back();
    if ( lastChunk != emptyChunk_ )
        std::swap( *emptyChunk_, *lastChunk );
    assert( lastChunk->HasAvailable( numBlocks_ ) );
    lastChunk->Release();
    chunks_.pop_back();

    if ( chunks_.empty() )
    {
        allocChunk_ = NULL;
        deallocChunk_ = NULL;
    }
    else
    {
        if ( deallocChunk_ == emptyChunk_ )
        {
            deallocChunk_ = &chunks_.front();
            assert( deallocChunk_->blocksAvailable_ < numBlocks_ );
        }
        if ( allocChunk_ == emptyChunk_ )
        {
            allocChunk_ = &chunks_.back();
            assert( allocChunk_->blocksAvailable_ < numBlocks_ );
        }
    }

    emptyChunk_ = NULL;
    assert( 0 == CountEmptyChunks() );

    return true;
}

// FixedAllocator::TrimChunkList ----------------------------------------------

bool FixedAllocator::TrimChunkList( void )
{
    if ( chunks_.empty() )
    {
        assert( NULL == allocChunk_ );
        assert( NULL == deallocChunk_ );
    }

    if ( chunks_.size() == chunks_.capacity() )
        return false;
    // Use the "make-a-temp-and-swap" trick to remove excess capacity.
    Chunks( chunks_ ).swap( chunks_ );

    return true;
}

// FixedAllocator::MakeNewChunk -----------------------------------------------

bool FixedAllocator::MakeNewChunk( void )
{
    bool allocated = false;
    try
    {
        std::size_t size = chunks_.size();
        // Calling chunks_.reserve *before* creating and initializing the new
        // Chunk means that nothing is leaked by this function in case an
        // exception is thrown from reserve.
        if ( chunks_.capacity() == size )
        {
            if ( 0 == size ) size = 4;
            chunks_.reserve( size * 2 );
        }
        Chunk newChunk;
        allocated = newChunk.Init( blockSize_, numBlocks_ );
        if ( allocated )
            chunks_.push_back( newChunk );
    }
    catch ( ... )
    {
        allocated = false;
    }
    if ( !allocated ) return false;

    allocChunk_ = &chunks_.back();
    deallocChunk_ = &chunks_.front();
    return true;
}

// FixedAllocator::Allocate ---------------------------------------------------

void * FixedAllocator::Allocate( void )
{
    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
    assert( CountEmptyChunks() < 2 );

    if ( ( NULL == allocChunk_ ) || allocChunk_->IsFilled() )
    {
        if ( NULL != emptyChunk_ )
        {
            allocChunk_ = emptyChunk_;
            emptyChunk_ = NULL;
        }
        else
        {
            for ( ChunkIter i( chunks_.begin() ); ; ++i )
            {
                if ( chunks_.end() == i )
                {
                    if ( !MakeNewChunk() )
                        return NULL;
                    break;
                }
                if ( !i->IsFilled() )
                {
                    allocChunk_ = &*i;
                    break;
                }
            }
        }
    }
    else if ( allocChunk_ == emptyChunk_)
        // detach emptyChunk_ from allocChunk_, because after 
        // calling allocChunk_->Allocate(blockSize_); the chunk 
        // is no longer empty.
        emptyChunk_ = NULL;

    assert( allocChunk_ != NULL );
    assert( !allocChunk_->IsFilled() );
    void * place = allocChunk_->Allocate( blockSize_ );

    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
    assert( CountEmptyChunks() < 2 );
#ifdef LOKI_CHECK_FOR_CORRUPTION
    if ( allocChunk_->IsCorrupt( numBlocks_, blockSize_, true ) )
    {
        assert( false );
        return NULL;
    }
#endif

    return place;
}

// FixedAllocator::Deallocate -------------------------------------------------

bool FixedAllocator::Deallocate( void * p, Chunk * hint )
{
    assert(!chunks_.empty());
    assert(&chunks_.front() <= deallocChunk_);
    assert(&chunks_.back() >= deallocChunk_);
    assert( &chunks_.front() <= allocChunk_ );
    assert( &chunks_.back() >= allocChunk_ );
    assert( CountEmptyChunks() < 2 );

    Chunk * foundChunk = ( NULL == hint ) ? VicinityFind( p ) : hint;
    if ( NULL == foundChunk )
        return false;

    assert( foundChunk->HasBlock( p, numBlocks_ * blockSize_ ) );
#ifdef LOKI_CHECK_FOR_CORRUPTION
    if ( foundChunk->IsCorrupt( numBlocks_, blockSize_, true ) )
    {
        assert( false );
        return false;
    }
    if ( foundChunk->IsBlockAvailable( p, numBlocks_, blockSize_ ) )
    {
        assert( false );
        return false;
    }
#endif
    deallocChunk_ = foundChunk;
    DoDeallocate(p);
    assert( CountEmptyChunks() < 2 );

    return true;
}

// FixedAllocator::VicinityFind -----------------------------------------------

Chunk * FixedAllocator::VicinityFind( void * p ) const
{
    if ( chunks_.empty() ) return NULL;
    assert(deallocChunk_);

    const std::size_t chunkLength = numBlocks_ * blockSize_;
    Chunk * lo = deallocChunk_;
    Chunk * hi = deallocChunk_ + 1;
    const Chunk * loBound = &chunks_.front();
    const Chunk * hiBound = &chunks_.back() + 1;

    // Special case: deallocChunk_ is the last in the array
    if (hi == hiBound) hi = NULL;

    for (;;)
    {
        if (lo)
        {
            if ( lo->HasBlock( p, chunkLength ) ) return lo;
            if ( lo == loBound )
            {
                lo = NULL;
                if ( NULL == hi ) break;
            }
            else --lo;
        }

        if (hi)
        {
            if ( hi->HasBlock( p, chunkLength ) ) return hi;
            if ( ++hi == hiBound )
            {
                hi = NULL;
                if ( NULL == lo ) break;
            }
        }
    }

    return NULL;
}

// FixedAllocator::DoDeallocate -----------------------------------------------

void FixedAllocator::DoDeallocate(void* p)
{
    // Show that deallocChunk_ really owns the block at address p.
    assert( deallocChunk_->HasBlock( p, numBlocks_ * blockSize_ ) );
    // Either of the next two assertions may fail if somebody tries to
    // delete the same block twice.
    assert( emptyChunk_ != deallocChunk_ );
    assert( !deallocChunk_->HasAvailable( numBlocks_ ) );
    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );

    // call into the chunk, will adjust the inner list but won't release memory
    deallocChunk_->Deallocate(p, blockSize_);

    if ( deallocChunk_->HasAvailable( numBlocks_ ) )
    {
        assert( emptyChunk_ != deallocChunk_ );
        // deallocChunk_ is empty, but a Chunk is only released if there are 2
        // empty chunks.  Since emptyChunk_ may only point to a previously
        // cleared Chunk, if it points to something else besides deallocChunk_,
        // then FixedAllocator currently has 2 empty Chunks.
        if ( NULL != emptyChunk_ )
        {
            // If last Chunk is empty, just change what deallocChunk_
            // points to, and release the last.  Otherwise, swap an empty
            // Chunk with the last, and then release it.
            Chunk * lastChunk = &chunks_.back();
            if ( lastChunk == deallocChunk_ )
                deallocChunk_ = emptyChunk_;
            else if ( lastChunk != emptyChunk_ )
                std::swap( *emptyChunk_, *lastChunk );
            assert( lastChunk->HasAvailable( numBlocks_ ) );
            lastChunk->Release();
            chunks_.pop_back();
            if ( ( allocChunk_ == lastChunk ) || allocChunk_->IsFilled() ) 
                allocChunk_ = deallocChunk_;
        }
        emptyChunk_ = deallocChunk_;
    }

    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
}

// GetOffset ------------------------------------------------------------------
/// @ingroup SmallObjectGroupInternal
/// Calculates index into array where a FixedAllocator of numBytes is located.
inline std::size_t GetOffset( std::size_t numBytes, std::size_t alignment )
{
    const std::size_t alignExtra = alignment-1;
    return ( numBytes + alignExtra ) / alignment;
}

// DefaultAllocator -----------------------------------------------------------
/** @ingroup SmallObjectGroupInternal
 Calls the default allocator when SmallObjAllocator decides not to handle a
 request.  SmallObjAllocator calls this if the number of bytes is bigger than
 the size which can be handled by any FixedAllocator.
 @param numBytes number of bytes
 @param doThrow True if this function should throw an exception, or false if it
  should indicate failure by returning a NULL pointer.
*/
void * DefaultAllocator( std::size_t numBytes, bool doThrow )
{
#ifdef USE_NEW_TO_ALLOCATE
    return doThrow ? ::operator new( numBytes ) :
        ::operator new( numBytes, std::nothrow_t() );
#else
    void * p = ::std::malloc( numBytes );
    if ( doThrow && ( NULL == p ) )
        throw std::bad_alloc();
    return p;
#endif
}

// DefaultDeallocator ---------------------------------------------------------
/** @ingroup SmallObjectGroupInternal
 Calls default deallocator when SmallObjAllocator decides not to handle a   
 request.  The default deallocator could be the global delete operator or the
 free function.  The free function is the preferred default deallocator since
 it matches malloc which is the preferred default allocator.  SmallObjAllocator
 will call this if an address was not found among any of its own blocks.
 */
void DefaultDeallocator( void * p )
{
#ifdef USE_NEW_TO_ALLOCATE
    ::operator delete( p );
#else
    ::std::free( p );
#endif
}

// SmallObjAllocator::SmallObjAllocator ---------------------------------------

SmallObjAllocator::SmallObjAllocator( std::size_t pageSize,
    std::size_t maxObjectSize, std::size_t objectAlignSize ) :
    pool_( NULL ),
    maxSmallObjectSize_( maxObjectSize ),
    objectAlignSize_( objectAlignSize )
{
#ifdef DO_EXTRA_LOKI_TESTS
    std::cout << "SmallObjAllocator " << this << std::endl;
#endif
    assert( 0 != objectAlignSize );
    const std::size_t allocCount = GetOffset( maxObjectSize, objectAlignSize );
    pool_ = new FixedAllocator[ allocCount ];
    for ( std::size_t i = 0; i < allocCount; ++i )
        pool_[ i ].Initialize( ( i+1 ) * objectAlignSize, pageSize );
}

// SmallObjAllocator::~SmallObjAllocator --------------------------------------

SmallObjAllocator::~SmallObjAllocator( void )
{
#ifdef DO_EXTRA_LOKI_TESTS
    std::cout << "~SmallObjAllocator " << this << std::endl;
#endif
    delete [] pool_;
}

// SmallObjAllocator::TrimExcessMemory ----------------------------------------

bool SmallObjAllocator::TrimExcessMemory( void )
{
    bool found = false;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    std::size_t i = 0;
    for ( ; i < allocCount; ++i )
    {
        if ( pool_[ i ].TrimEmptyChunk() )
            found = true;
    }
    for ( i = 0; i < allocCount; ++i )
    {
        if ( pool_[ i ].TrimChunkList() )
            found = true;
    }

    return found;
}

// SmallObjAllocator::Allocate ------------------------------------------------

void * SmallObjAllocator::Allocate( std::size_t numBytes, bool doThrow )
{
    if ( numBytes > GetMaxObjectSize() )
        return DefaultAllocator( numBytes, doThrow );

    assert( NULL != pool_ );
    if ( 0 == numBytes ) numBytes = 1;
    const std::size_t index = GetOffset( numBytes, GetAlignment() ) - 1;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    (void) allocCount;
    assert( index < allocCount );

    FixedAllocator & allocator = pool_[ index ];
    assert( allocator.BlockSize() >= numBytes );
    assert( allocator.BlockSize() < numBytes + GetAlignment() );
    void * place = allocator.Allocate();

    if ( ( NULL == place ) && TrimExcessMemory() )
        place = allocator.Allocate();

    if ( ( NULL == place ) && doThrow )
    {
#ifdef _MSC_VER
        throw std::bad_alloc( "could not allocate small object" );
#else
        // GCC did not like a literal string passed to std::bad_alloc.
        // so just throw the default-constructed exception.
        throw std::bad_alloc();
#endif
    }
    return place;
}

// SmallObjAllocator::Deallocate ----------------------------------------------

void SmallObjAllocator::Deallocate( void * p, std::size_t numBytes )
{
    if ( NULL == p ) return;
    if ( numBytes > GetMaxObjectSize() )
    {
        DefaultDeallocator( p );
        return;
    }
    assert( NULL != pool_ );
    if ( 0 == numBytes ) numBytes = 1;
    const std::size_t index = GetOffset( numBytes, GetAlignment() ) - 1;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    (void) allocCount;
    assert( index < allocCount );
    FixedAllocator & allocator = pool_[ index ];
    assert( allocator.BlockSize() >= numBytes );
    assert( allocator.BlockSize() < numBytes + GetAlignment() );
    const bool found = allocator.Deallocate( p, NULL );
    (void) found;
    assert( found );
}

// SmallObjAllocator::Deallocate ----------------------------------------------

void SmallObjAllocator::Deallocate( void * p )
{
    if ( NULL == p ) return;
    assert( NULL != pool_ );
    FixedAllocator * pAllocator = NULL;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    Chunk * chunk = NULL;

    for ( std::size_t ii = 0; ii < allocCount; ++ii )
    {
        chunk = pool_[ ii ].HasBlock( p );
        if ( NULL != chunk )
        {
            pAllocator = &pool_[ ii ];
            break;
        }
    }
    if ( NULL == pAllocator )
    {
        DefaultDeallocator( p );
        return;
    }

    assert( NULL != chunk );
    const bool found = pAllocator->Deallocate( p, chunk );
    (void) found;
    assert( found );
}

// SmallObjAllocator::IsCorrupt -----------------------------------------------

bool SmallObjAllocator::IsCorrupt( void ) const
{
    if ( NULL == pool_ )
    {
        assert( false );
        return true;
    }
    if ( 0 == GetAlignment() )
    {
        assert( false );
        return true;
    }
    if ( 0 == GetMaxObjectSize() )
    {
        assert( false );
        return true;
    }
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    for ( std::size_t ii = 0; ii < allocCount; ++ii )
    {
        if ( pool_[ ii ].IsCorrupt() )
            return true;
    }
    return false;
}
