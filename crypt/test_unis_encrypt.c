#include "unis_encrypt.h"



int main(){

	AES_ENCODE_DATA test ={0};
	test.textBuffer = "yangshengyuan";
	test.bufferSize = strlen(test.textBuffer);
	unis_aes_encrypt(&test);

	if (test.enBuffer != NULL)
	{
		size_t _buf_len;
		oaes_sprintf( NULL, &_buf_len, test.enBuffer, test.bufferSize );
		char *_buf = (char *) calloc(_buf_len, sizeof( char));
		printf( "\n***** cyphertext  *****\n" );
		if( _buf )
		{
			oaes_sprintf( _buf, &_buf_len, test.enBuffer, test.bufferSize );
			printf( "%s", _buf );
		}
		printf( "\n**********************\n\n" );
		free( _buf );
	}


	AES_ENCODE_DATA test2 ={0};
	test2.enBuffer = test.enBuffer;
	test2.bufferSize = test.bufferSize;

	unis_aes_decrypt(&test2);

	if (test2.textBuffer != NULL)
	{
		size_t _buf_len;
		oaes_sprintf( NULL, &_buf_len, test.textBuffer, test.bufferSize );
		char *_buf = (char *) calloc(_buf_len, sizeof( char));
		printf( "\n***** plaintext  *****\n" );
		if( _buf )
		{
			oaes_sprintf( _buf, &_buf_len, test.textBuffer, test.bufferSize );
			printf( "%s", _buf );
		}
		printf( "\n%s\n",test2.textBuffer);
		printf( "\n**********************\n\n" );
		free( _buf );
	}

	free(test.enBuffer);
	test.enBuffer= NULL;
	test2.enBuffer= NULL;


    return 1;

}
