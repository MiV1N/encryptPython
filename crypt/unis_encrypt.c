#include <stdlib.h>
#include "unis_encrypt.h"
#include "oaes_lib.h"
#include "secret_key.h"  // auto generate at build time

typedef struct _oaes_key
{
	size_t data_len;
	uint8_t *data;
	size_t exp_data_len;
	uint8_t *exp_data;
	size_t num_keys;
	size_t key_base;
} oaes_key;

typedef struct _oaes_ctx
{
#ifdef OAES_HAVE_ISAAC
  randctx * rctx;
#endif // OAES_HAVE_ISAAC

#ifdef OAES_DEBUG
	oaes_step_cb step_cb;
#endif // OAES_DEBUG

	oaes_key * key;
	OAES_OPTION options;
	uint8_t iv[OAES_BLOCK_SIZE];
} oaes_ctx;

static void KeyAlloc(OAES_CTX** key)
{
    *key = oaes_alloc();
    oaes_key_gen_128(*key);
    oaes_key_import_data(*key, GetSecretKey(), SECRET_KEY_LEN);
}


int unis_aes_encrypt(AES_ENCODE_DATA *plainTextData)
{
    OAES_CTX * ctx = NULL;
	uint8_t *_encbuf;
	size_t _encbuf_len,_text_len;
	char *_buf;
	short _is_ecb = 0;
	unsigned char * _text= NULL;
	int _key_len = 128;

    _text = plainTextData->textBuffer;
    _text_len = plainTextData->bufferSize;

    // ctx = oaes_alloc();
	KeyAlloc(&ctx);


	if( NULL == ctx )
	{
		printf("Error: Failed to initialize OAES.\n");
		free( _text );
		return EXIT_FAILURE;
	}
#ifdef DEBUG
	{

		size_t _buf_len;
		oaes_sprintf( NULL, &_buf_len, ((oaes_ctx *)ctx)->key->data, ((oaes_ctx *)ctx)->key->data_len );
		char *_buf = (char *) calloc(_buf_len, sizeof( char));
		printf( "\n***** key  *****\n" );
		if( _buf )
		{
			oaes_sprintf( _buf, &_buf_len, ((oaes_ctx *)ctx)->key->data, ((oaes_ctx *)ctx)->key->data_len );
			printf( "%s", _buf );
		}
		printf( "\n**********************\n\n" );
		free( _buf );
	}
#endif
    if( OAES_RET_SUCCESS != oaes_encrypt( ctx,  (const uint8_t *)_text, _text_len, NULL, &_encbuf_len ) )
    printf("Error: Failed to retrieve required buffer size for encryption.\n");

	_encbuf = (uint8_t *) calloc( _encbuf_len, sizeof(uint8_t) );
	if( NULL == _encbuf )
	{
		printf( "Error: Failed to allocate memory.\n" );
		free( _text );
		return EXIT_FAILURE;
	}

	if( OAES_RET_SUCCESS != oaes_encrypt( ctx, (const uint8_t *)_text, _text_len, _encbuf, &_encbuf_len ) )
		printf("Error: Encryption failed.\n");

	// printf("%ld\n",_encbuf_len);
	// return 1;
    plainTextData->enBuffer = _encbuf;
    plainTextData->bufferSize= _encbuf_len;

    if( OAES_RET_SUCCESS !=  oaes_free( &ctx ) )
		printf("Error: Failed to uninitialize OAES.\n");

	return (EXIT_SUCCESS);

}

int free_endecbuf(uint8_t * buffer)
{
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	return (EXIT_SUCCESS);
}

int unis_aes_decrypt(AES_ENCODE_DATA *plainTextData)
{
    OAES_CTX * ctx = NULL;
	uint8_t *_encbuf, *_decbuf;
	size_t _encbuf_len, _decbuf_len, _buf_len;
	short _is_ecb = 0;
	int _key_len = 128;

    _encbuf = plainTextData->enBuffer;
    _encbuf_len= plainTextData->bufferSize;

	KeyAlloc(&ctx);
    // ctx = oaes_alloc();
	if( NULL == ctx )
	{
		printf("Error: Failed to initialize OAES.\n");
		return EXIT_FAILURE;
	}
#ifdef DEBUG
	{

		size_t _buf_len;
		oaes_sprintf( NULL, &_buf_len, (((oaes_ctx *)ctx))->key->data, ((oaes_ctx *)ctx)->key->data_len );
		char *_buf = (char *) calloc(_buf_len, sizeof( char));
		printf( "\n***** key  *****\n" );
		if( _buf )
		{
			oaes_sprintf( _buf, &_buf_len, ((oaes_ctx *)ctx)->key->data, ((oaes_ctx *)ctx)->key->data_len );
			printf( "%s", _buf );
		}
		printf( "\n**********************\n\n" );
		free( _buf );
	}
#endif

	int ret = oaes_decrypt( ctx, _encbuf, _encbuf_len, NULL, &_decbuf_len ) ;
	if( OAES_RET_SUCCESS != ret)
		printf("Error: Failed to retrieve required buffer size for encryption. errorcode:%d\n",ret);
	
	_decbuf = (uint8_t *) calloc( _decbuf_len, sizeof(uint8_t) );
	if( NULL == _decbuf )
	{
		printf( "Error: Failed to allocate memory.\n" );
		free( _encbuf );
		return EXIT_FAILURE;
	}

	if( OAES_RET_SUCCESS != oaes_decrypt( ctx,_encbuf, _encbuf_len, _decbuf, &_decbuf_len ) )
		printf("Error: Decryption failed.\n");

#ifdef DEBUG
	if (_decbuf != NULL)
	{
		size_t _buf_len;
		oaes_sprintf( NULL, &_buf_len, _decbuf, _decbuf_len );
		char *_buf = (char *) calloc(_buf_len, sizeof( char));
		printf( "\n***** plaintext  *****\n" );
		if( _buf )
		{
			oaes_sprintf( _buf, &_buf_len, _decbuf, _decbuf_len );
			printf( "%s", _buf );
		}
		printf( "\n**********************\n\n" );
	}
#endif

	if( OAES_RET_SUCCESS !=  oaes_free( &ctx ) )
		printf("Error: Failed to uninitialize OAES.\n");

    plainTextData->textBuffer = _decbuf;
    plainTextData->bufferSize = _decbuf_len;

	return (EXIT_SUCCESS);

}
