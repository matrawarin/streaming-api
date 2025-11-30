fun main() {
    while(true){
        // 1. อัปเดตราคาหุ้นจากเซิร์ฟเวอร์
        ApiClient.api.updatePrices().execute()
        val stocks = ApiClient.api.getStocks().execute().body() ?: emptyList()

        // 2. รันกลยุทธ์อัตโนมัติ
        autoTradeEngine(stocks)

        // 3. แสดงพอร์ตโฟลิโอ
        val portfolio = ApiClient.api.getPortfolio().execute().body() ?: emptyList()
        println("--- Portfolio ---")
        for (p in portfolio) {
            println("${p.symbol}: ${p.shares} shares, Avg ${p.avgPrice}")
        }

        // 4. หน่วงเวลา 10 วินาที (จำลองรอบตลาด)
        Thread.sleep(10000)
    }
}

