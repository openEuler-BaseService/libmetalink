/* <!-- copyright */
/*
 * libmetalink
 *
 * Copyright (c) 2008 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/* copyright --> */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "metalink_types.h"
#include "metalink_error.h"

#include <stdio.h>

metalink_error_t allocate_copy_string(char** dest, const char* src)
{
  free(*dest);
  if(src) {
    size_t length;
    length = strlen(src)+1;
    *dest = malloc(length);
    if(*dest == NULL) {
      return METALINK_ERR_BAD_ALLOC;
    }
    memcpy(*dest, src, length);
    return 0;
  } else {
    *dest = NULL;
    return 0;
  }
}

metalink_file_t* new_metalink_file()
{
  metalink_file_t* file;
  file = malloc(sizeof(metalink_file_t));
  if(file) {
    memset(file, 0, sizeof(metalink_file_t));
  }
  return file;
}

void delete_metalink_file(metalink_file_t* file)
{
  metalink_resource_t** res;
  metalink_checksum_t** checksums;
  if(file) {
    free(file->name);
    free(file->version);
    free(file->language);
    free(file->os);

    if(file->resources) {
      res = file->resources;
      while(*res) {
	delete_metalink_resource(*res);
	++res;
      }
      free(file->resources);
    }

    if(file->checksums) {
      checksums = file->checksums;
      while(*checksums) {
	delete_metalink_checksum(*checksums);
	++checksums;
      }
      free(file->checksums);
    }

    delete_metalink_chunk_checksum(file->chunk_checksum);

    free(file);
  }
}

metalink_error_t metalink_file_set_name(metalink_file_t* file, const char* name)
{
  return allocate_copy_string(&file->name, name);
}

void metalink_file_set_size(metalink_file_t* file, long long int size)
{
  file->size = size;
}

metalink_error_t metalink_file_set_version(metalink_file_t* file, const char* version)
{
  return allocate_copy_string(&file->version, version);
}

metalink_error_t metalink_file_set_language(metalink_file_t* file, const char* language)
{
  return allocate_copy_string(&file->language, language);
}

metalink_error_t metalink_file_set_os(metalink_file_t* file, const char* os)
{
  return allocate_copy_string(&file->os, os);
}

void metalink_file_set_maxconnections(metalink_file_t* file, int maxconnections)
{
  file->maxconnections = maxconnections;
}

/* for metalink_resource_t */

metalink_resource_t* new_metalink_resource()
{
  metalink_resource_t* resource;
  resource = malloc(sizeof(metalink_resource_t));
  if(resource) {
    memset(resource, 0, sizeof(metalink_resource_t));
  }
  return resource;
}

void delete_metalink_resource(metalink_resource_t* resource)
{
  if(resource) {
    free(resource->url);
    free(resource->type);
    free(resource->location);
    free(resource);
  }
}

metalink_error_t metalink_resource_set_type(metalink_resource_t* resource, const char* type)
{
  return allocate_copy_string(&resource->type, type);
}

metalink_error_t metalink_resource_set_location(metalink_resource_t* resource,
				   const char* location)
{
  return allocate_copy_string(&resource->location, location);
}

void metalink_resource_set_preference(metalink_resource_t* resource,
				      int preference)
{
  resource->preference = preference;
}

void metalink_resource_set_maxconnections(metalink_resource_t* resource,
					  int maxconnections)
{
  resource->maxconnections = maxconnections;
}

metalink_error_t metalink_resource_set_url(metalink_resource_t* resource, const char* url)
{
  return allocate_copy_string(&resource->url, url);
}

/* for metalink_checksum_t */
metalink_checksum_t* new_metalink_checksum()
{
  metalink_checksum_t* checksum;
  checksum = malloc(sizeof(metalink_checksum_t));
  if(checksum) {
    memset(checksum, 0, sizeof(metalink_checksum_t));
  }
  return checksum;
}

void delete_metalink_checksum(metalink_checksum_t* checksum)
{
  if(checksum) {
    free(checksum->type);
    free(checksum->hash);
    free(checksum);
  }
}

metalink_error_t metalink_checksum_set_type(metalink_checksum_t* checksum, const char* type)
{
  return allocate_copy_string(&checksum->type, type);
}

metalink_error_t metalink_checksum_set_hash(metalink_checksum_t* checksum, const char* hash)
{
  return allocate_copy_string(&checksum->hash, hash);
}

/* for metalink_piece_hash_t */
metalink_piece_hash_t* new_metalink_piece_hash()
{
  metalink_piece_hash_t* piece_hash;
  piece_hash = malloc(sizeof(metalink_piece_hash_t));
  if(piece_hash) {
    memset(piece_hash, 0, sizeof(metalink_piece_hash_t));
  }
  return piece_hash;
}

void delete_metalink_piece_hash(metalink_piece_hash_t* piece_hash)
{
  if(!piece_hash) {
    return;
  }
  free(piece_hash->hash);
  free(piece_hash);
}

void metalink_piece_hash_set_piece(metalink_piece_hash_t* piece_hash, int piece)
{
  piece_hash->piece = piece;
}

metalink_error_t metalink_piece_hash_set_hash(metalink_piece_hash_t* piece_hash, const char* hash)
{
  return allocate_copy_string(&piece_hash->hash, hash);
}

/* for metalink_chunk_checksum_t */
metalink_chunk_checksum_t* new_metalink_chunk_checksum()
{
  metalink_chunk_checksum_t* chunk_checksum;
  chunk_checksum = malloc(sizeof(metalink_chunk_checksum_t));
  if(chunk_checksum) {
    memset(chunk_checksum, 0, sizeof(metalink_chunk_checksum_t));
  }
  return chunk_checksum;
}

void delete_metalink_chunk_checksum(metalink_chunk_checksum_t* chunk_checksum)
{
  metalink_piece_hash_t** p;
  if(!chunk_checksum) {
    return;
  }
  free(chunk_checksum->type);
  if(chunk_checksum->piece_hashes) {
    p = chunk_checksum->piece_hashes;
    while(*p) {
      delete_metalink_piece_hash(*p);
      ++p;
    }
    free(chunk_checksum->piece_hashes);
  }
  free(chunk_checksum);
}

metalink_error_t metalink_chunk_checksum_set_type(metalink_chunk_checksum_t* chunk_checksum,
				     const char* type)
{
  return allocate_copy_string(&chunk_checksum->type, type);
}

void metalink_chunk_checksum_set_length(metalink_chunk_checksum_t* chunk_checksum,
					int length)
{
  chunk_checksum->length = length;
}

void metalink_chunk_checksum_set_piece_hashes
(metalink_chunk_checksum_t* chunk_checksum,
 metalink_piece_hash_t** piece_hashes)
{
  if(chunk_checksum->piece_hashes) {
    metalink_piece_hash_t** p;
    p = chunk_checksum->piece_hashes;
    while(*p) {
      delete_metalink_piece_hash(*p);
      ++p;
    }
  }
  chunk_checksum->piece_hashes = piece_hashes;
}

/* for metalink_t */
metalink_t* new_metalink()
{
  metalink_t* metalink;
  metalink = malloc(sizeof(metalink_t));
  if(metalink) {
    memset(metalink, 0, sizeof(metalink_t));
  }
  return metalink;
}

void delete_metalink(metalink_t* metalink)
{
  metalink_file_t** filepp;
  if(!metalink) {
    return;
  }
  if(metalink->files) {
    filepp = metalink->files;
    while(*filepp) {
      delete_metalink_file(*filepp);
      ++filepp;
    }
    free(metalink->files);
  }
  free(metalink);
}
