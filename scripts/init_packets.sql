DROP TABLE IF EXISTS packets;
CREATE TABLE packets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    protocol TEXT,
    src_ip TEXT,
    src_port INTEGER,
    dst_ip TEXT,
    dst_port INTEGER,
    size INTEGER
);
