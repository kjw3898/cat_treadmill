/*
 * circular_buffer.c
 *
 *  Created on: 2019. 12. 16.
 *      Author: issacs
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "circular_buffer.h"
cbuf_handle_t cbuf;

static void advance_pointer(cbuf_handle_t cbuf)
{
	assert(cbuf);

	if(cbuf->full)
    {
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    }

	cbuf->head = (cbuf->head + 1) % cbuf->max;

	// We mark full because we will advance tail on the next time around
	cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(cbuf_handle_t cbuf)
{
	assert(cbuf);

	cbuf->full = false;
	cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}

cbuf_handle_t circular_buf_init(exReport_handle_t buffer, size_t size)
{
	assert(buffer && size);

	cbuf_handle_t cbuf = malloc(sizeof(circular_buf_t));
	assert(cbuf);

	cbuf->buffer = buffer;
	cbuf->max = size;
	circular_buf_reset(cbuf);

	assert(circular_buf_empty(cbuf));

	return cbuf;
}

void circular_buf_free(cbuf_handle_t cbuf)
{
	assert(cbuf);
	free(cbuf);
}

void circular_buf_reset(cbuf_handle_t cbuf)
{
    assert(cbuf);

    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
}

size_t circular_buf_size(cbuf_handle_t cbuf)
{
	assert(cbuf);

	size_t size = cbuf->max;

	if(!cbuf->full)
	{
		if(cbuf->head >= cbuf->tail)
		{
			size = (cbuf->head - cbuf->tail);
		}
		else
		{
			size = (cbuf->max + cbuf->head - cbuf->tail);
		}

	}

	return size;
}

size_t circular_buf_capacity(cbuf_handle_t cbuf)
{
	assert(cbuf);

	return cbuf->max;
}

void circular_buf_put(cbuf_handle_t cbuf, exReport_handle_t data)
{
	assert(cbuf && cbuf->buffer);

    memcpy(&cbuf->buffer[cbuf->head], data, sizeof(exerciseReport));

    advance_pointer(cbuf);
}

int circular_buf_put_non_overwrite(cbuf_handle_t cbuf, exReport_handle_t data)
{
    int r = -1;

    assert(cbuf && cbuf->buffer);

    if(!circular_buf_full(cbuf))
    {
        memcpy(&cbuf->buffer[cbuf->head], data, sizeof(exerciseReport));
        advance_pointer(cbuf);
        r = 0;
    }

    return r;
}

int circular_buf_get(cbuf_handle_t cbuf, exReport_handle_t data)
{
    assert(cbuf && cbuf->buffer);

    int r = -1;

    if(!circular_buf_empty(cbuf))
    {
//        data = cbuf->buffer[cbuf->tail];
        memcpy(data, &cbuf->buffer[cbuf->head], sizeof(exerciseReport));
        retreat_pointer(cbuf);

        r = 0;
    }

    return r;
}

bool circular_buf_empty(cbuf_handle_t cbuf)
{
	assert(cbuf);

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

bool circular_buf_full(cbuf_handle_t cbuf)
{
	assert(cbuf);

    return cbuf->full;
}

int circular_buf_get_range(exReport_handle_t gbuf, cbuf_handle_t cbuf, size_t n)
{
	int i = 0, copyiedCnt = 0;

	assert(gbuf && cbuf && n);

	// data가 있는지 확인
	if(cbuf->head == cbuf->tail)
	{
		return 0;
	}
	else if(cbuf->head > cbuf->tail) //탐색 방법 1
	{
		for(i = cbuf->head-1; i >= cbuf->tail; i--)
		{



					//데이터 찾음
					memcpy(&gbuf[copyiedCnt], &cbuf->buffer[i], sizeof(exerciseReport));
					copyiedCnt += 1;
					if(copyiedCnt == n)
						return copyiedCnt;


		}
	}
	else
	{
		for(i = cbuf->head-1; i >= 0; i--)  //탐색 방법 3
				{

					memcpy(&gbuf[copyiedCnt], &cbuf->buffer[i], sizeof(exerciseReport));
												copyiedCnt += 1;
												if(copyiedCnt == n)
													return copyiedCnt;

				}
		for(i = cbuf->max-1; i >= cbuf->tail; i--)  //탐색 방법 2
		{

			//데이터 찾음
								memcpy(&gbuf[copyiedCnt], &cbuf->buffer[i], sizeof(exerciseReport));
								copyiedCnt += 1;
								if(copyiedCnt == n)
									return copyiedCnt;

		}



	}

	//요청한 데이터보다 더 많은 데이터를 요구했을 경우 복사한 만큼 리턴
	if(i == cbuf->head)
	{
		return copyiedCnt;
	}

	return 0;
}

void print_buffer_status(cbuf_handle_t cbuf)
{
	printf("\rFull: %d, empty: %d, size: %d\n",
				 circular_buf_full(cbuf),
				 circular_buf_empty(cbuf),
				 circular_buf_size(cbuf));
}
