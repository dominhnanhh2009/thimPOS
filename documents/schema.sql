CREATE TABLE IF NOT EXISTS products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    name TEXT NOT NULL,
    price INTEGER NOT NULL CHECK(price >= 0),
    description TEXT NOT NULL DEFAULT '',

    is_active INTEGER NOT NULL DEFAULT 1 CHECK(is_active IN (0, 1)),

    created_at INTEGER NOT NULL,
    updated_at INTEGER NOT NULL,

    image_url TEXT,
    category TEXT NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS orders (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at INTEGER NOT NULL,
    updated_at INTEGER NOT NULL,
    total_price INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS order_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    order_id   INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    quantity   INTEGER NOT NULL,
    unit_price INTEGER NOT NULL,
    line_total INTEGER NOT NULL,
    created_at INTEGER NOT NULL,

    FOREIGN KEY(order_id) REFERENCES orders(id)
);