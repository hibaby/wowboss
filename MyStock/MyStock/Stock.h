#pragma once

#include <string>
#include <vector>

class CStock
{
public:
	struct TradeData {
		std::string		date;
		unsigned int	open;
		unsigned int	high;
		unsigned int	low;
		unsigned int	close;
		unsigned int	volume;
		unsigned int	adjClose;
	};
public:
	CStock(std::string& id);
	~CStock(void);

	void AddTradeData(const TradeData& td);
	void AddKeyDate();

	//Technical Analysis
	//Technical index

private:
	std::string m_id;
	std::vector< TradeData > m_TradeDatas;
};

