/*
 * trades.c
 * Simple terminal-based stock trades table manager (single-file).
 *
 * Features:
 *  - Add / Edit / Delete trades
 *  - List trades (with pagination)
 *  - Filter by symbol, side (BUY/SELL)
 *  - Sort by date, symbol, quantity, price
 *  - Save to / Load from CSV
 *  - Compute totals and basic stats
 *
 * Compile:
 *   gcc -std=c11 -O2 -Wall -o trades trades.c
 *
 * Run:
 *   ./trades
 *
 * CSV columns:
 * id,symbol,side,quantity,price,date,notes
 *
 * Notes:
 *  - Simple date stored as string (YYYY-MM-DD).
 *  - Uses dynamic array (realloc).
 *  - No external libraries.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 64
#define LINEBUF_SIZE 1024

typedef enum { SIDE_BUY = 0, SIDE_SELL = 1 } Side;

typedef struct {
    long id;                // unique id
    char symbol[32];        // stock symbol
    Side side;              // BUY/SELL
    long quantity;          // number of shares
    double price;           // price per share
    char date[16];          // YYYY-MM-DD
    char notes[128];        // optional notes
} Trade;

typedef struct {
    Trade *data;
    size_t size;
    size_t capacity;
    long next_id;
} TradeTable;

/* ---------- Utilities ---------- */

static void trim_newline(char *s) {
    size_t i = strlen(s);
    if (i == 0) return;
    if (s[i-1] == '\n') s[i-1] = '\0';
    if (i >= 2 && s[i-2] == '\r') s[i-2] = '\0';
}

static char *read_line_stdin(const char *prompt, char *buf, size_t bufsz) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (!fgets(buf, (int)bufsz, stdin)) {
        buf[0] = '\0';
        return NULL;
    }
    trim_newline(buf);
    return buf;
}

static void to_upper_inplace(char *s) {
    while (*s) { *s = (char)toupper((unsigned char)*s); s++; }
}

/* ---------- TradeTable ops ---------- */

static void table_init(TradeTable *t) {
    t->data = malloc(sizeof(Trade) * INITIAL_CAPACITY);
    t->size = 0;
    t->capacity = (t->data ? INITIAL_CAPACITY : 0);
    t->next_id = 1;
}

static void table_free(TradeTable *t) {
    free(t->data);
    t->data = NULL;
    t->size = 0;
    t->capacity = 0;
    t->next_id = 1;
}

static int ensure_capacity(TradeTable *t, size_t mincap) {
    if (t->capacity >= mincap) return 1;
    size_t newcap = t->capacity ? t->capacity * 2 : INITIAL_CAPACITY;
    while (newcap < mincap) newcap *= 2;
    Trade *p = realloc(t->data, sizeof(Trade) * newcap);
    if (!p) return 0;
    t->data = p;
    t->capacity = newcap;
    return 1;
}

static Trade *table_add(TradeTable *t, const Trade *tr) {
    if (!ensure_capacity(t, t->size + 1)) return NULL;
    t->data[t->size] = *tr;
    t->size++;
    return &t->data[t->size - 1];
}

static Trade *table_find_by_id(TradeTable *t, long id) {
    for (size_t i=0;i<t->size;i++) if (t->data[i].id == id) return &t->data[i];
    return NULL;
}

static int table_delete_by_id(TradeTable *t, long id) {
    size_t i;
    for (i=0;i<t->size;i++) if (t->data[i].id == id) break;
    if (i == t->size) return 0;
    // move last element here
    if (i != t->size - 1) t->data[i] = t->data[t->size - 1];
    t->size--;
    return 1;
}

/* ---------- CSV IO ---------- */

static int trade_to_csv_line(char *buf, size_t bufsz, const Trade *tr) {
    // escape quotes by doubling them for notes field if needed
    char notes_esc[512] = {0};
    size_t idx = 0;
    const char *p = tr->notes;
    for (; *p && idx + 2 < sizeof(notes_esc); ++p) {
        if (*p == '"') {
            notes_esc[idx++] = '"';
            notes_esc[idx++] = '"';
        } else {
            notes_esc[idx++] = *p;
        }
    }
    notes_esc[idx] = '\0';
    // CSV: id,symbol,side,quantity,price,date,notes
    snprintf(buf, bufsz, "%ld,%s,%s,%ld,%.10g,%s,\"%s\"",
             tr->id,
             tr->symbol,
             (tr->side==SIDE_BUY) ? "BUY" : "SELL",
             tr->quantity,
             tr->price,
             tr->date,
             notes_esc);
    return 1;
}

static int save_csv(TradeTable *t, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fprintf(f, "id,symbol,side,quantity,price,date,notes\n");
    char line[1024];
    for (size_t i=0;i<t->size;i++) {
        trade_to_csv_line(line, sizeof(line), &t->data[i]);
        fprintf(f, "%s\n", line);
    }
    fclose(f);
    return 1;
}

// Very simple CSV line parser - assumes well-formed lines from our own writer
static int parse_csv_line(const char *line, Trade *out) {
    // copy line to modifiable buffer
    char buf[LINEBUF_SIZE];
    strncpy(buf, line, LINEBUF_SIZE-1); buf[LINEBUF_SIZE-1] = '\0';
    // naive parse: split first 6 commas, last field may have quotes and commas
    char *fields[7] = {0};
    int fi = 0;
    char *p = buf;
    char *start = p;
    int in_quotes = 0;
    for (; *p && fi < 6; ++p) {
        if (*p == '"') in_quotes = !in_quotes;
        else if (*p == ',' && !in_quotes) {
            *p = '\0';
            fields[fi++] = start;
            start = p + 1;
        }
    }
    if (fi < 6) {
        // maybe fewer fields -> fail
        return 0;
    }
    // remaining is last field
    fields[6] = start;

    // strip optional surrounding quotes from field 6 (notes)
    for (int i=0;i<7;i++) if (fields[i]) {
        char *s = fields[i];
        // trim leading spaces
        while (*s == ' ') s++;
        // update pointer
        fields[i] = s;
    }

    // parse each field
    out->id = atol(fields[0]);
    strncpy(out->symbol, fields[1], sizeof(out->symbol)-1); out->symbol[sizeof(out->symbol)-1] = '\0';
    to_upper_inplace(out->symbol);
    char *side_s = fields[2];
    if (strcmp(side_s, "BUY") == 0) out->side = SIDE_BUY;
    else out->side = SIDE_SELL;
    out->quantity = atol(fields[3]);
    out->price = atof(fields[4]);
    strncpy(out->date, fields[5], sizeof(out->date)-1); out->date[sizeof(out->date)-1] = '\0';
    // notes: remove leading/trailing quotes and unescape double quotes
    char *notes = fields[6];
    size_t nlen = strlen(notes);
    if (nlen >= 2 && notes[0] == '"' && notes[nlen-1] == '"') {
        notes[nlen-1] = '\0';
        notes++;
    }
    // unescape double quotes
    char notes_unesc[128] = {0};
    size_t idx = 0;
    for (size_t k=0; notes[k] && idx + 1 < sizeof(notes_unesc); ++k) {
        if (notes[k] == '"' && notes[k+1] == '"') {
            notes_unesc[idx++] = '"'; k++;
        } else {
            notes_unesc[idx++] = notes[k];
        }
    }
    notes_unesc[idx] = '\0';
    strncpy(out->notes, notes_unesc, sizeof(out->notes)-1); out->notes[sizeof(out->notes)-1] = '\0';
    return 1;
}

static int load_csv(TradeTable *t, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    char line[LINEBUF_SIZE];
    // read header
    if (!fgets(line, sizeof(line), f)) { fclose(f); return 0; }
    trim_newline(line);
    // read rows
    size_t loaded = 0;
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (strlen(line) == 0) continue;
        Trade tr;
        if (!parse_csv_line(line, &tr)) {
            // skip malformed
            continue;
        }
        // ensure id unique: set next_id to max(id)+1
        if (!ensure_capacity(t, t->size + 1)) { fclose(f); return 0; }
        t->data[t->size++] = tr;
        if (tr.id >= t->next_id) t->next_id = tr.id + 1;
        loaded++;
    }
    fclose(f);
    return (int)loaded;
}

/* ---------- Commands / UI ---------- */

static void print_trade(const Trade *tr) {
    printf("%6ld | %-6s | %-4s | %8ld | %10.4f | %10s | %s\n",
           tr->id,
           tr->symbol,
           (tr->side==SIDE_BUY) ? "BUY" : "SELL",
           tr->quantity,
           tr->price,
           tr->date,
           tr->notes);
}

static void list_trades(TradeTable *t) {
    if (t->size == 0) {
        printf("No trades.\n");
        return;
    }
    printf("   ID  | Symbol | Side | Quantity |     Price |      Date | Notes\n");
    printf("--------------------------------------------------------------------------\n");
    for (size_t i=0;i<t->size;i++) {
        print_trade(&t->data[i]);
    }
    printf("--------------------------------------------------------------------------\n");
    printf("Total rows: %zu\n", t->size);
}

static void add_trade_interactive(TradeTable *t) {
    char buf[LINEBUF_SIZE];
    Trade tr;
    memset(&tr, 0, sizeof(tr));
    tr.id = t->next_id++;

    // symbol
    read_line_stdin("Symbol (e.g. AAPL): ", buf, sizeof(buf));
    while (strlen(buf) == 0) {
        read_line_stdin("Symbol (required): ", buf, sizeof(buf));
    }
    strncpy(tr.symbol, buf, sizeof(tr.symbol)-1); tr.symbol[sizeof(tr.symbol)-1] = '\0';
    to_upper_inplace(tr.symbol);

    // side
    read_line_stdin("Side (BUY/SELL) [BUY]: ", buf, sizeof(buf));
    if (strlen(buf) == 0) strcpy(buf, "BUY");
    to_upper_inplace(buf);
    if (strcmp(buf, "SELL") == 0) tr.side = SIDE_SELL; else tr.side = SIDE_BUY;

    // quantity
    read_line_stdin("Quantity (integer): ", buf, sizeof(buf));
    tr.quantity = atol(buf);

    // price
    read_line_stdin("Price (per share): ", buf, sizeof(buf));
    tr.price = atof(buf);

    // date
    read_line_stdin("Date (YYYY-MM-DD): ", buf, sizeof(buf));
    if (strlen(buf) == 0) strcpy(buf, "1970-01-01");
    strncpy(tr.date, buf, sizeof(tr.date)-1); tr.date[sizeof(tr.date)-1] = '\0';

    // notes
    read_line_stdin("Notes (optional): ", buf, sizeof(buf));
    strncpy(tr.notes, buf, sizeof(tr.notes)-1); tr.notes[sizeof(tr.notes)-1] = '\0';

    if (!table_add(t, &tr)) {
        fprintf(stderr, "Failed to add trade (memory).\n");
    } else {
        printf("Added trade id=%ld\n", tr.id);
    }
}

static void edit_trade_interactive(TradeTable *t) {
    char buf[LINEBUF_SIZE];
    read_line_stdin("Enter trade id to edit: ", buf, sizeof(buf));
    long id = atol(buf);
    Trade *tr = table_find_by_id(t, id);
    if (!tr) { printf("Trade id %ld not found.\n", id); return; }
    printf("Editing trade (leave empty to keep current value):\n");
    printf("Current: "); print_trade(tr);

    // symbol
    read_line_stdin("Symbol: ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        strncpy(tr->symbol, buf, sizeof(tr->symbol)-1); tr->symbol[sizeof(tr->symbol)-1] = '\0';
        to_upper_inplace(tr->symbol);
    }
    // side
    read_line_stdin("Side (BUY/SELL): ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        to_upper_inplace(buf);
        if (strcmp(buf, "SELL") == 0) tr->side = SIDE_SELL; else tr->side = SIDE_BUY;
    }
    // quantity
    read_line_stdin("Quantity: ", buf, sizeof(buf));
    if (strlen(buf) > 0) tr->quantity = atol(buf);
    // price
    read_line_stdin("Price: ", buf, sizeof(buf));
    if (strlen(buf) > 0) tr->price = atof(buf);
    // date
    read_line_stdin("Date (YYYY-MM-DD): ", buf, sizeof(buf));
    if (strlen(buf) > 0) strncpy(tr->date, buf, sizeof(tr->date)-1);
    // notes
    read_line_stdin("Notes: ", buf, sizeof(buf));
    if (strlen(buf) > 0) strncpy(tr->notes, buf, sizeof(tr->notes)-1);

    printf("Updated: "); print_trade(tr);
}

static void delete_trade_interactive(TradeTable *t) {
    char buf[LINEBUF_SIZE];
    read_line_stdin("Enter trade id to delete: ", buf, sizeof(buf));
    long id = atol(buf);
    if (table_delete_by_id(t, id)) {
        printf("Deleted trade id %ld\n", id);
    } else {
        printf("Trade id %ld not found.\n", id);
    }
}

static int cmp_by_symbol(const void *a, const void *b) {
    const Trade *ta = a, *tb = b;
    return strcmp(ta->symbol, tb->symbol);
}
static int cmp_by_date(const void *a, const void *b) {
    const Trade *ta = a, *tb = b;
    return strcmp(ta->date, tb->date);
}
static int cmp_by_quantity(const void *a, const void *b) {
    const Trade *ta = a, *tb = b;
    if (ta->quantity < tb->quantity) return -1;
    if (ta->quantity > tb->quantity) return 1;
    return 0;
}
static int cmp_by_price(const void *a, const void *b) {
    const Trade *ta = a, *tb = b;
    if (ta->price < tb->price) return -1;
    if (ta->price > tb->price) return 1;
    return 0;
}

static void sort_interactive(TradeTable *t) {
    if (t->size < 2) { printf("Not enough rows to sort.\n"); return; }
    char buf[LINEBUF_SIZE];
    printf("Sort by: 1) symbol 2) date 3) quantity 4) price\n");
    read_line_stdin("Choose (1-4): ", buf, sizeof(buf));
    int choice = atoi(buf);
    switch (choice) {
        case 1: qsort(t->data, t->size, sizeof(Trade), cmp_by_symbol); break;
        case 2: qsort(t->data, t->size, sizeof(Trade), cmp_by_date); break;
        case 3: qsort(t->data, t->size, sizeof(Trade), cmp_by_quantity); break;
        case 4: qsort(t->data, t->size, sizeof(Trade), cmp_by_price); break;
        default: printf("Invalid choice.\n"); return;
    }
    printf("Sorted.\n");
}

static void filter_interactive(TradeTable *t) {
    if (t->size == 0) { printf("No trades.\n"); return; }
    char buf[LINEBUF_SIZE];
    read_line_stdin("Filter symbol (leave empty to skip): ", buf, sizeof(buf));
    char symbol[32] = {0};
    if (strlen(buf) > 0) { strncpy(symbol, buf, sizeof(symbol)-1); to_upper_inplace(symbol); }

    read_line_stdin("Filter side (BUY/SELL or empty): ", buf, sizeof(buf));
    Side side_filter = -1;
    if (strlen(buf) > 0) {
        to_upper_inplace(buf);
        if (strcmp(buf, "BUY") == 0) side_filter = SIDE_BUY;
        else if (strcmp(buf, "SELL") == 0) side_filter = SIDE_SELL;
    }

    printf("   ID  | Symbol | Side | Quantity |     Price |      Date | Notes\n");
    printf("--------------------------------------------------------------------------\n");
    size_t count = 0;
    for (size_t i=0;i<t->size;i++) {
        int match = 1;
        if (symbol[0] != '\0' && strcmp(t->data[i].symbol, symbol) != 0) match = 0;
        if (side_filter != -1 && t->data[i].side != side_filter) match = 0;
        if (match) { print_trade(&t->data[i]); count++; }
    }
    printf("--------------------------------------------------------------------------\n");
    printf("Matched rows: %zu\n", count);
}

static void stats_interactive(TradeTable *t) {
    if (t->size == 0) { printf("No trades.\n"); return; }
    // compute totals per symbol (simple)
    typedef struct {
        char symbol[32];
        long net_qty;   // positive for net buy, negative for net sell
        double avg_price; // average price for net position (weighted) - naive
        double cash_flow; // buys negative, sells positive
    } SymStat;
    SymStat *arr = NULL;
    size_t arrsz = 0;
    for (size_t i=0;i<t->size;i++) {
        Trade *tr = &t->data[i];
        // find symbol
        size_t j;
        for (j=0;j<arrsz;j++) if (strcmp(arr[j].symbol, tr->symbol) == 0) break;
        if (j == arrsz) {
            // add new
            SymStat tmp;
            memset(&tmp,0,sizeof(tmp));
            strncpy(tmp.symbol, tr->symbol, sizeof(tmp.symbol)-1);
            tmp.net_qty = 0;
            tmp.avg_price = 0.0;
            tmp.cash_flow = 0.0;
            arr = realloc(arr, sizeof(SymStat) * (arrsz + 1));
            arr[arrsz] = tmp;
            arrsz++;
        }
        // update
        double val = tr->quantity * tr->price;
        if (tr->side == SIDE_BUY) {
            // buying increases net quantity and reduces cash (negative flow)
            double prev_qty = (double)arr[j].net_qty;
            double prev_cost = arr[j].avg_price * prev_qty;
            double new_qty = prev_qty + (double)tr->quantity;
            double new_cost = prev_cost + val;
            arr[j].net_qty += tr->quantity;
            if (new_qty != 0) arr[j].avg_price = new_cost / new_qty;
            arr[j].cash_flow -= val;
        } else {
            // sell reduces net_qty, increases cash
            double prev_qty = (double)arr[j].net_qty;
            double prev_cost = arr[j].avg_price * prev_qty;
            // simple approach: when selling, reduce net_qty and keep avg price same for remaining (not FIFO)
            arr[j].net_qty -= tr->quantity;
            arr[j].cash_flow += val;
        }
    }
    printf("Symbol | NetQty | AvgPrice | CashFlow\n");
    printf("----------------------------------------\n");
    for (size_t i=0;i<arrsz;i++) {
        printf("%-6s | %6ld | %8.4f | %10.2f\n",
               arr[i].symbol, arr[i].net_qty, arr[i].avg_price, arr[i].cash_flow);
    }
    free(arr);
}

/* ---------- Main menu ---------- */

static void print_help() {
    printf("Commands:\n");
    printf("  a  - add trade\n");
    printf("  e  - edit trade\n");
    printf("  d  - delete trade\n");
    printf("  l  - list trades\n");
    printf("  f  - filter/search\n");
    printf("  s  - sort\n");
    printf("  save <file> - save CSV\n");
    printf("  load <file> - load CSV (append)\n");
    printf("  stats - show per-symbol stats\n");
    printf("  q  - quit\n");
    printf("  h  - help\n");
}

int main(void) {
    TradeTable table;
    table_init(&table);

    char cmdline[LINEBUF_SIZE];
    char token[256];
    printf("Trades table manager (C) - simple terminal app\n");
    print_help();

    for (;;) {
        printf("\n> ");
        if (!fgets(cmdline, sizeof(cmdline), stdin)) break;
        trim_newline(cmdline);
        if (strlen(cmdline) == 0) continue;
        // parse first token
        token[0] = '\0';
        sscanf(cmdline, "%255s", token);
        to_upper_inplace(token);

        if (strcmp(token, "A") == 0 || strcmp(token, "ADD") == 0) {
            add_trade_interactive(&table);
        } else if (strcmp(token, "E") == 0 || strcmp(token, "EDIT") == 0) {
            edit_trade_interactive(&table);
        } else if (strcmp(token, "D") == 0 || strcmp(token, "DEL") == 0 || strcmp(token, "DELETE") == 0) {
            delete_trade_interactive(&table);
        } else if (strcmp(token, "L") == 0 || strcmp(token, "LIST") == 0) {
            list_trades(&table);
        } else if (strcmp(token, "F") == 0 || strcmp(token, "FILTER") == 0 || strcmp(token, "SEARCH") == 0) {
            filter_interactive(&table);
        } else if (strcmp(token, "S") == 0 || strcmp(token, "SORT") == 0) {
            sort_interactive(&table);
        } else if (strcmp(token, "SAVE") == 0) {
            char filename[512] = {0};
            if (sscanf(cmdline + strlen(token), " %511s", filename) <= 0) {
                printf("Usage: save filename.csv\n");
            } else {
                if (save_csv(&table, filename)) printf("Saved to %s\n", filename);
                else printf("Failed to save to %s\n", filename);
            }
        } else if (strcmp(token, "LOAD") == 0) {
            char filename[512] = {0};
            if (sscanf(cmdline + strlen(token), " %511s", filename) <= 0) {
                printf("Usage: load filename.csv\n");
            } else {
                int n = load_csv(&table, filename);
                if (n < 0) printf("Failed to load %s\n", filename);
                else printf("Loaded %d rows from %s\n", n, filename);
            }
        } else if (strcmp(token, "STATS") == 0) {
            stats_interactive(&table);
        } else if (strcmp(token, "H") == 0 || strcmp(token, "HELP") == 0) {
            print_help();
        } else if (strcmp(token, "Q") == 0 || strcmp(token, "QUIT") == 0 || strcmp(token, "EXIT") == 0) {
            break;
        } else {
            printf("Unknown command. Type 'h' for help.\n");
        }
    }

    table_free(&table);
    printf("Goodbye.\n");
    return 0;
}

