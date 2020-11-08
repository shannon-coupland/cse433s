#!/usr/bin/python3
# code shell adapted from https://en.bitcoin.it/wiki/Block_hashing_algorithm
#  by Steve Cole for cse433s Spring 2019#  last revised Fall 2020
# NAME:
# Fill in the code sections marked # YOUR CODE HERE . You may use any of the
#  utility functions contained in this file to help you.
import hashlib
import sys

### UTILITY FUNCTIONS ###
def switch_endian(hex_string):    
    ''' 
    Takes in a string representing hex-encoded bytes (2 chars per byte)    
    and returns a copy of the string with the byte ordering reversed.    
    Example: converts 'a1b2c3d4' to 'd4c3b2a1' .    
    @param hex_string The hex-string-encoded number to be reversed    
    @return A bytewise-reversed copy of hex_string    
    '''    
    hex_val = bytes.fromhex(hex_string)    
    rev = hex_val[::-1].hex()    
    return rev

def int32_to_little_endian(n):    
    ''' 
    Converts a 4-byte integer value to a hex-encoded string (2 chars per byte)      
    in Little Endian order.    
    Pads with zeroes if less than 8 chars.    
    Example: converts 26 to '1a000000'    
    @param n Integer to be converted    
    @return Little Endian hex string version of integer    
    '''    
    return switch_endian('{:08x}'.format(n)) 
    
def int256_to_hex_str(t):    
    '''    
    Takes 32-byte numerical value and returns it encoded as       
    a hex-encoded string with 2 chars per byte.    
    Example: takes in 33 and returns '00000..21' (64 chars total).    
    Useful for printing threshold as a string.    
    '''
    return t.hex()

def extract_threshold(bits):    
    '''    
    Extracts a numerical threshold from the 'bits' field of a Bitcoin block.    
    Note: 'bits' field is assumed to be a string in Little Endian order       
    (as it's shown in the Blockchain viewer for Bitcoin).    
    @param bits 'bits' field of a Bitcoin block    
    @return Numerical threshold encoded in the 'bits' field    
    '''
    # convert hex string and reverse    
    bits_be = switch_endian(bits)    
    bits_num = int(bits_be, 16)   

    # extract threshold from 4-byte 'bits' field    
    # exponent: MSB    
    # mantissa: 3 remaining bytes     
    # threshold = mantissa * 2**(8*(exponent-3))    
    e = bits_num >> 24    
    m = bits_num & 0xFFFFFF    
    t = m * (1 << (8 * (e - 3)))    
    return t
    
### CORE FUNCTIONS ###
def hash_block(version, prev_hash, merkle_root, ts, bits, nonce):    
    '''    
    Computes the hash value for a Bitcoin block with the given parameters.    
    See https://en.bitcoin.it/wiki/Block_hashing_algorithm for details      
    on the Bitcoin hash algorithm.    
    Note that parameters must be in Little Endian format.    
    @param version Bitcoin version    
    @param prev_hash Hash of previous block     
    @param merkle_root Root of Merkle tree for the block     
    @param ts Timestamp    
    @param bits 'bits' field of block; encodes threshold below which hash value must be    
    @param nonce 4-byte value that causes hash to be below threshold extracted frombits    
    @return String representing Big Endian hex encoding of hash value    
    '''    

    header_hex = (version + prev_hash + merkle_root + ts + bits + nonce)
    header_bin = bytes.fromhex(header_hex) #same as decode('hex')
    hashy = hashlib.sha256(hashlib.sha256(header_bin).digest()).digest()
    return int256_to_hex_str(hashy[::-1])

    
def mine_block(version, prev_hash, merkle_root, ts, bits):    
    '''    
    Computes a 4-byte nonce value that will yield a valid Bitcoin block given       
    the other header values passed as parameters.    
    See https://en.bitcoin.it/wiki/Block_hashing_algorithm for details      
    on the Bitcoin hash algorithm.    
    Note that parameters must be in Little Endian format.    
    @param version: Bitcoin version    
    @param prev_hash: Hash of previous block     
    @param merkle_root: root of Merkle tree for the block     
    @param ts: timestamp    
    @param bits: Bits field of block; encodes threshold below which hash value must be    
    @return nonce value    
    '''

    threshold = extract_threshold(bits) 
    nonce = 0xb0000000
    while (True):
        nonce_little_string = int32_to_little_endian(nonce)
        hashy = hash_block(version, prev_hash, merkle_root, ts, bits, nonce_little_string) #returns big endian hex string

        if (int(hashy, 16) < threshold):
            print("Found hash " + hashy + " for nonce int " + str(nonce) + ", nonce little endian string " + nonce_little_string)
            return nonce

        if (nonce > 0xc0000000):
            print("not found :(")
            return -1

        nonce = nonce + 1
    
if __name__ == "__main__":     
    print('Example from wiki:')
    print(hash_block("01000000", "81cd02ab7e569e8bcd9317e2fe99f2de44d49ab2b8851ba4a308000000000000", "e320b6c2fffc8d750423db8b1eb942ae710e951ed797f7affc8892b0f1fc122b", "c7f5d74d", "f2b9441a", "42a14695"))
    
    print("\nAnswer to task 1:")
    print(hash_block("01000000", "99ba9de222aeae9fff2d33df2e147b693e94568eb21643f0eb19000000000000", "d60fbe4a6f19d7558052ddbe2986fa1e3e02166c4e61d68c1da5361b45dad70b", "569ade4d", "f2b9441a", "59ad690d"))
    
    print("\nAnswer to task 2:")
    mine_block("01000000", "0935dc5fe42633dcc3329651a66a58d32381eb8484c28ec7a827000000000000", "80458a3423c112590b20e8c32750279f5d3276b0a689e2b183011c9ec821daa9", "0c9ede4d", "f2b9441a")