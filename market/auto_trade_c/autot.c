#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_STOCKS 5
#define MAX_NAME_LEN 20

typedef struct {
    char symbol[MAX_NAME_LEN];
    double price;
    int shares; // shares held in portfolio
    double avg_price; // average buy price
} Stock;

void initStocks(Stock stocks[], int n) {
    const char* symbols[MAX_STOCKS] = {"AAPL", "GOOG", "MSFT", "TSLA", "AMZN"};
    for(int i=0;i<n;i++){
        strncpy(stocks[i].symbol, symbols[i], MAX_NAME_LEN);
        stocks[i].price = 50 + rand()%50; // initial price 50-100
        stocks[i].shares = 0;
        stocks[i].avg_price = 0.0;
    }
}

void updatePrices(Stock stocks[], int n) {
    for(int i=0;i<n;i++){
        double change = ((rand()%2000)/1000.0 - 1.0); // -1.0 to +1.0
        stocks[i].price += change;
        if(stocks[i].price < 1) stocks[i].price = 1;
    }
}

void autoTrade(Stock stocks[], int n) {
    for(int i=0;i<n;i++){
        // Example strategy: Buy <60, Sell >90
        if(stocks[i].price < 60) {
            int qty = 10;
            double total = stocks[i].avg_price*stocks[i].shares + stocks[i].price*qty;
            stocks[i].shares += qty;
            stocks[i].avg_price = total / stocks[i].shares;
            printf("BUY  %s x%d at %.2f\n", stocks[i].symbol, qty, stocks[i].price);
        } else if(stocks[i].price > 90 && stocks[i].shares > 0){
            int qty = stocks[i].shares;
            printf("SELL %s x%d at %.2f (Profit: %.2f)\n", 
                    stocks[i].symbol, qty, stocks[i].price,
                    (stocks[i].price - stocks[i].avg_price)*qty);
            stocks[i].shares = 0;
            stocks[i].avg_price = 0;
        }
    }
}

void showPortfolio(Stock stocks[], int n) {
    printf("\n--- Portfolio ---\n");
    printf("Symbol\tShares\tAvgPrice\tCurrentPrice\tValue\n");
    for(int i=0;i<n;i++){
        if(stocks[i].shares > 0){
            double value = stocks[i].shares * stocks[i].price;
            printf("%s\t%d\t%.2f\t\t%.2f\t\t%.2f\n", 
                stocks[i].symbol, stocks[i].shares, stocks[i].avg_price, stocks[i].price, value);
        }
    }
}

int main() {
    srand(time(NULL));
    Stock stocks[MAX_STOCKS];
    initStocks(stocks, MAX_STOCKS);

    for(int day=1; day<=30; day++){ // simulate 30 days
        printf("\n=== Day %d ===\n", day);
        updatePrices(stocks, MAX_STOCKS);
        for(int i=0;i<MAX_STOCKS;i++){
            printf("%s: %.2f\n", stocks[i].symbol, stocks[i].price);
        }
        autoTrade(stocks, MAX_STOCKS);
        showPortfolio(stocks, MAX_STOCKS);
        printf("----------------------------\n");
        getchar(); // wait for user input each day
    }

    printf("\n=== Final Portfolio Value ===\n");
    double totalValue = 0;
    for(int i=0;i<MAX_STOCKS;i++){
        totalValue += stocks[i].shares * stocks[i].price;
    }
    printf("Total Portfolio Value: %.2f\n", totalValue);

    return 0;
}

