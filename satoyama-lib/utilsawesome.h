#ifdef __cplusplus
extern "C" {
#endif

typedef struct Reading{
	char *name;
	double value;
	long timestamp;
};

void add_to_tx_buf(char *tx_buf, struct Reading *reading);

#ifdef __cplusplus
}
#endif