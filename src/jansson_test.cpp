#include <jansson.h>

/*
 * Parse text into a JSON object. If text is valid JSON, returns a
 * json_t structure, otherwise prints and error and returns null.
 */
json_t *load_json(const char *text) {
    json_t *root;
    json_error_t error;

    root = json_loads(text, 0, &error);

    if (root) {
        return root;
    } else {
        fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
        return (json_t *)0;
    }
}

int main(int argc, char* argv[]){

	/* parse text into JSON structure */
    json_t *root = load_json("{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getblockchaininfo\", \"params\": [] }");
	printf("Hello World!\n");
}