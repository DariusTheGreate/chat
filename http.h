#ifndef _HTTP_
#define _HTTP_

class http_request{
public:
	http_request(char* address);
	~http_request();
	void parse_html(int sock, char* filename);

private:	
	char method_type[16];
	char path[2048];
	char proto[16];
	uint8_t state;
	uint8_t index;
};

class HTTP{
private:
	char* host;
	int32_t len;
	int32_t capacity;
};


#endif
