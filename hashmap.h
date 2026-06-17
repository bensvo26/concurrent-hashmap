typedef struct {
int key;
int value;
} item;

int init(int size);

void put(item kv_item);

item* get(int key);