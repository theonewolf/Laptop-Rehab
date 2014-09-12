#!/usr/bin/env python

from hashlib import sha256 as HASH
from sys import argv as arguments

CHUNK_SIZE  =   4096

def hash_chunker(stream):
    chunk = stream.read(CHUNK_SIZE)
    while chunk:
        yield HASH(chunk).digest()
        chunk = stream.read(CHUNK_SIZE)


if __name__ == '__main__':
    f1name = arguments[1]
    f2name = arguments[2]

    f1hashes = set()
    f2hashes = set()

    with open(f1name, 'r') as f1:
        with open(f2name, 'r') as f2:
            for chunk in hash_chunker(f1):
                f1hashes.add(chunk)
            for chunk in hash_chunker(f2):
                f2hashes.add(chunk)

    print 'Hashes in %s: %d' % (f1name, len(f1hashes))
    print 'Hashes in %s: %d' % (f2name, len(f2hashes))
    
    print 'Hashes in intersection: %d' % (len(f1hashes - f2hashes))
