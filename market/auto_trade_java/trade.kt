fun autoTradeEngine(stocks: List<Stock>) {
    for (stock in stocks) {
        // ตัวอย่างกลยุทธ์: ซื้อหากราคาต่ำกว่า 50, ขายหากราคาสูงกว่า 100
        if (stock.price < 50) {
            ApiClient.api.trade(stock.symbol, "BUY", 10).execute()
            println("BUY ${stock.symbol} x10 at ${stock.price}")
        } else if (stock.price > 100) {
            // ขายทั้งหมดในพอร์ต
            val portfolio = ApiClient.api.getPortfolio().execute().body() ?: emptyList()
            val item = portfolio.find { it.symbol == stock.symbol }
            if (item != null && item.shares > 0) {
                ApiClient.api.trade(stock.symbol, "SELL", item.shares).execute()
                println("SELL ${stock.symbol} x${item.shares} at ${stock.price}")
            }
        }
    }
}

