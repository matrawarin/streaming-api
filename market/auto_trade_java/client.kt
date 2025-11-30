data class Stock(
    val symbol: String,
    val name: String,
    var price: Double
)

data class PortfolioItem(
    val symbol: String,
    var shares: Int,
    var avgPrice: Double
)

