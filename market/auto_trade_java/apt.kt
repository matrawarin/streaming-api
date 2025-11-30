import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.GET
import retrofit2.http.POST
import retrofit2.http.Query
import retrofit2.Call

interface StockApi {
    @GET("api/stocks.php")
    fun getStocks(): Call<List<Stock>>

    @GET("api/portfolio.php")
    fun getPortfolio(): Call<List<PortfolioItem>>

    @POST("api/trade.php")
    fun trade(
        @Query("symbol") symbol: String,
        @Query("type") type: String,
        @Query("shares") shares: Int
    ): Call<String>

    @POST("api/auto_trade.php")
    fun autoTrade(): Call<String>

    @POST("api/update_prices.php")
    fun updatePrices(): Call<String>
}

object ApiClient {
    private val retrofit = Retrofit.Builder()
        .baseUrl("https://yourserver.com/") // เปลี่ยนเป็น URL ของคุณ
        .addConverterFactory(GsonConverterFactory.create())
        .build()

    val api: StockApi = retrofit.create(StockApi::class.java)
}

