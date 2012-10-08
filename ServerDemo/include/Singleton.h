#pragma once

#include <cassert>

/// Should be placed in the appropriate .cpp file somewhere
#define initialiseSingleton( type ) \
  template <> type * Singleton < type > :: mSingleton = 0

template < class type > class  Singleton
{
	public:

		inline static type & getSingleton() { assert(mSingleton); return *mSingleton; }
		inline static type* getSingletonPtr() { return mSingleton; }
		static void initialise()
		{
			/// If you hit this assert, this singleton already exists -- you can't create another one!
			assert(mSingleton == 0);
			mSingleton = new type();
		}
		static void release()
		{
			/// If you hit this assert, this singleton already exists -- you can't create another one!
			if( mSingleton )
			{
				delete mSingleton;
				mSingleton = NULL;
			}
		}

	protected:
		/// Constructor
		Singleton() {}
		/// Destructor
		virtual ~Singleton() {}

		/// Singleton pointer, must be set to 0 prior to creating the object
		static type* mSingleton;
};
