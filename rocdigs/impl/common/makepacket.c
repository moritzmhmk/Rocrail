/**
 * Provide utilities for coding/decoding NMRA S&RP DCC packets into
 * sequences to send through a standard serial port.
 * <P>
 * This is strongly based on the makepckt.c file
 * from the PacketScript 1.1. package of Kenneth Rice. The original header
 * comment from that file follows here.
 *
 *
 * <pre>
 *
 * Some Useful Background information
 *
 *      startbit: 1
 *      stopbit : 1
 *      databits: 8
 *      baudrate: 19200
 *
 *      ==> one serial bit takes 52.08 usec. (at 19200 baud)
 *
 *      ==> NMRA-1-Bit: 01         (52 usec low and 52 usec high)
 *          NMRA-0-Bit: 0011       (at least 100 usec low and high)
 *           note we are allowed to stretch NMRA-0 e.g. 000111,
 *           00001111, 000001111
 *          are all valid NMRA-0 representations
 *
 *      serial stream (only start/stop bits):
 *
 *      0_______10_______10_______10_______10_______10_______10___ ...
 *
 *      problem: how to place the NMRA-0- and NMRA-1-Bits in the
 *    serial stream
 *
 *     examples:
 *
 *      0          0xF0     _____-----
 *      00         0xC6     __--___---
 *      01         0x78     ____----_-
 *      10         0xE1     _-____----
 *      001        0x66     __--__--_-
 *      010        0x96     __--_-__--
 *      011        0x5C     ___---_-_-
 *      100        0x99     _-__--__--
 *      101        0x71     _-___---_-
 *      110        0xC5     _-_-___---
 *      0111       0x56     __--_-_-_-
 *      1011       0x59     _-__--_-_-
 *      1101       0x65     _-_-__--_-
 *      1110       0x95     _-_-_-__--
 *      11111      0x55     _-_-_-_-_-
 *                          ^        ^
 *                          start-   stop-
 *                          bit      bit
 *
 * Limitation
 * If we ever need to generate a pattern of four '1's followed by a '0' and
 * land it on a the start of a byte boundary - Sorry - it can't be done !!
 *
 *
 * makepckt.c
 *
 * Send an nmra packet out the serial port in such a way that the signal can
 * just be amplified and put on the track.
 *
 * Copyright 1993 Kenneth Rice
 *
 * You may freely distribute this source code, and use all or part of
 * this source code in all software including commercial, freeware,
 * shareware and private applications.
 *
 * Please report bugs, fixes, etc to me at:
 *  kenr@xis.xerox.com
 * or
 *  73577,1653 (compuserve)
 *
 * Created  02/08/93
 *      03/05/93  Works for all 3 byte packets. Still errors for 4 byte.
 *      07/01/93  Renamed to makepckt.c to be nice to dos users.
 *      10/23/93  Added backtracking and max length.
 * </pre>
 *
 * @author   Bob Jacobsen Copyright (C) 2001
 * @version  $Revision: 1.9 $
 *
 */
#include <stdio.h>

#include "rocs/public/mem.h"
#include "rocs/public/str.h"

#include "nmrapacket.h"


static int preambleLength = 15;
/* This should be a multiple of 5. */
#define BITSTREAM_BITS_PER_BYTE 9
/* number of bits per byte/
/* nmra   s01234567s (hex equiv - note that in signal, 0 bit is left) */
#define BITS_0     0xF0 /* 0      _____----- (0xF0) */
#define BITS_00    0xC6 /* 00     __--___--- (0xC6) */
#define BITS_01    0x78 /* 01     ____----_- (0x78) */
#define BITS_10    0xE1 /* 10     _-____---- (0xE1) */
#define BITS_001   0x66 /* 001    __--__--_- (0x66) */
#define BITS_010   0x96 /* 010    __--_-__-- (0x96) */
#define BITS_011   0x5C /* 011    ___---_-_- (0x5C) */
#define BITS_100   0x99 /* 100    _-__--__-- (0x99) */
#define BITS_101   0x71 /* 101    _-___---_- (0x71) */
#define BITS_110   0xC5 /* 110    _-_-___--- (0xC5) */
#define BITS_0111  0x56 /* 0111   __--_-_-_- (0x56) */
#define BITS_1011  0x59 /* 1011   _-__--_-_- (0x59) */
#define BITS_1101  0x65 /* 1101   _-_-__--_- (0x65) */
#define BITS_1110  0x95 /* 1110   _-_-_-__-- (0x95) */
#define BITS_11111 0x55 /* 11111  _-_-_-_-_- (0x55) */

struct node {
  int bitPattern;
  int patternLength;
};

static int bitStreamToSerialBytes(byte* serialStream, byte* inputBitStream, int inlen);
static Boolean readFirstChild(byte* bs, int offset, int validBits, struct node* thisNode);
static Boolean readNextChild(struct node* thisNode);


/**
 * Function that takes in the packet as a array of Bytes and converts
 * an them into NMRA'1','0' representation, in preparation to be sent over
 * a serial link.
 * @param packet byte[] - NRMA packet in a array of bytes
 * @return int[] - first byte is length - 0 length indicates failed to do
 */
int createStream(byte* serialStream, byte* packet, int packetsize) {
  int i, j = 0;
  int mask = 0x80;
  int bitStreamIndex = 0;
  byte bitStream[100] = {0};

  /* Make into an array of ints for easier processing. */
  /* do preamble */
  for (bitStreamIndex = 0; bitStreamIndex < preambleLength; bitStreamIndex++) {
    bitStream[bitStreamIndex] = 1;
    /* Add Packet Start Bit - 0 */
  }
  bitStream[bitStreamIndex++] = 0;
  /* Do packet bytes. */
  for (i = 0; i < packetsize; i++) {
    mask = 0x80;
    while (mask > 0) {
      bitStream[bitStreamIndex++] = (packet[i] & mask) != 0 ? 1 : 0;
      mask = (mask >> 1);
    }
    /* Add byte seperator - 0 */
    bitStream[bitStreamIndex++] = 0;
  }
  /* do end packet indicator */
  bitStream[--bitStreamIndex] = 1;

  /** So we now have a int array reflecting required packet
   * byte structure => bitStream
   */
  /**
   * Now do the hard part - convert this into a serial stream
   */
  return bitStreamToSerialBytes(serialStream, bitStream, bitStreamIndex);
}

/* BitStreamToSerialBytes
 *
 * Generate the serial bytes from the bit stream.
 * <P>
 * Basically this is a depth first, prune largest tree search, always going down the subtree
 * that uses the most bits for the next byte. If we get an error, backtrack up
 * the tree until we reach a node that we have not completely traversed all the
 * subtrees for and try going down the subtree that uses the second most bits.
 * Keep going until we finish converting the packet or run out of things to try.
 * <P>
 *
 * This is not guaranteed to find the shortest serial stream for a given
 * packet, but it is guaranteed to find a stream if one exists. Also, it
 * usually does come up with the shortest packet.
 */
static int bitStreamToSerialBytes(byte* serialStream, byte* inputBitStream, int inlen) {
  int currentBufferIndex;
  Boolean result;
  int treeIndex = -1;
  struct node tree[150];
  int outsize = 0;

  for (currentBufferIndex = 0; currentBufferIndex < 150; currentBufferIndex++) {
    tree[currentBufferIndex].bitPattern = 0;
    tree[currentBufferIndex].patternLength = 0;
  }

  /* Now generate the actual serial byte stream from the array of bits. */
  currentBufferIndex = 0;
  treeIndex = 1;
  while (currentBufferIndex < inlen) {
    if ( (result = readFirstChild(inputBitStream, currentBufferIndex,
        inlen - currentBufferIndex, &tree[treeIndex])))
    {
      /* Success, there is a Child at this level in the tree to read */
      /* Move down the tree to next node */
      serialStream[treeIndex] = tree[treeIndex].bitPattern;
      currentBufferIndex += tree[treeIndex++].patternLength;
    }
    /* Allow outer loop control to take us down next level; */
    else {
      /* Need to add some code to cope with stradling '1's to stop
                     backtrack from failure */
      if (currentBufferIndex + 4 > inlen) {
        serialStream[treeIndex] = BITS_11111;
        currentBufferIndex = inlen;
      }
      else {
        while (treeIndex > 0) {
          /* Inner loop to check all childs at this node */
          /* If no more childs then need to bracktrack */
          treeIndex--;
          currentBufferIndex -= tree[treeIndex].patternLength;
          if (readNextChild(&tree[treeIndex])) {
            serialStream[treeIndex] = tree[treeIndex].bitPattern;
            currentBufferIndex += tree[treeIndex++].patternLength;
            break;
          }
          if (treeIndex == 0) {
            serialStream[0] = 0;
            return inlen;
          }
        }
      }
    }
  }
  serialStream[0] = --treeIndex;
  return inlen;
}

/* ReadNextChild
 *
 * This routine find the next largest (ie longest lenght) child
 * at this node.
 * ThisNode - (INPUT/OUTPUT) determine if there is another child
 *                if so update node with ie the Bit
 *                pattern and its associated lenght.
 *
 * Return False if one doesnt exist otherwise returns True.
 */
static Boolean readNextChild(struct node* thisNode) {

  switch (thisNode->bitPattern) {
    /* Success - there is another child */

    case BITS_00:
    case BITS_01: {
      thisNode->bitPattern = BITS_0;
      thisNode->patternLength = 1;
      break;
    }
    case BITS_001: {
      thisNode->bitPattern = BITS_00;
      thisNode->patternLength = 2;
      break;
    }
    case BITS_010: {
      thisNode->bitPattern = BITS_01;
      thisNode->patternLength = 2;
      break;
    }
    case BITS_011: {
      thisNode->bitPattern = BITS_01;
      thisNode->patternLength = 2;
      break;
    }
    case BITS_100: {
      thisNode->bitPattern = BITS_10;
      thisNode->patternLength = 2;
      break;
    }
    case BITS_0111: {
      thisNode->bitPattern = BITS_011;
      thisNode->patternLength = 3;
      break;
    }
    case BITS_1011: {
      thisNode->bitPattern = BITS_101;
      thisNode->patternLength = 3;
      break;
    }
    case BITS_1101: {
      thisNode->bitPattern = BITS_110;
      thisNode->patternLength = 3;
      break;
    }
    /* We have exhausted all children on this level */
    default:
      return False;
  }
  return (True);
}

/* ReadFirstChild
 *
 * This routine find the first largest (ie longest length) child
 * at this node
 *
 * BS   - (INPUT) Bit stream array
 * offset       - Offset in to buffer
 * validBits  - (INPUT) number of valid bits in the bit stream.
 * ThisNode - (OUTPUT) where to put largest child found ie the Bit
 *                pattern and its associated lenght.
 *
 * Return False if one doesnt exist otherwise returns True.
 */
static Boolean readFirstChild(byte* bs, int offset, int validBits, struct node* thisNode) {
  Boolean b0 = False;
  Boolean b1 = False;
  Boolean b2 = False;
  Boolean b3 = False;
  Boolean b4 = False;
  thisNode->patternLength = 0;

  switch (validBits) { /* Note all cases fall through on purpose. */
    default:
      thisNode->patternLength = 5;
      b0 = (bs[0 + offset] != 0);
      b1 = (bs[1 + offset] != 0);
      b2 = (bs[2 + offset] != 0);
      b3 = (bs[3 + offset] != 0);
      b4 = (bs[4 + offset] != 0);
      if (b0 && b1 && b2 && b3 && b4) {
        thisNode->bitPattern = BITS_11111;
        break;
      }
    case 4:
      thisNode->patternLength = 4;
      b0 = (bs[0 + offset] != 0);
      b1 = (bs[1 + offset] != 0);
      b2 = (bs[2 + offset] != 0);
      b3 = (bs[3 + offset] != 0);
      if (!b0 && b1 && b2 && b3) {
        thisNode->bitPattern = BITS_0111;
        break;
      }
      if (b0 && !b1 && b2 && b3) {
        thisNode->bitPattern = BITS_1011;
        break;
      }
      if (b0 && b1 && !b2 && b3) {
        thisNode->bitPattern = BITS_1101;
        break;
      }
      if (b0 && b1 && b2 && !b3) {
        thisNode->bitPattern = BITS_1110;
        break;
      }
    case 3:
      b0 = (bs[0 + offset] != 0);
      b1 = (bs[1 + offset] != 0);
      b2 = (bs[2 + offset] != 0);
      thisNode->patternLength = 3;
      if (!b0 && !b1 && b2) {
        thisNode->bitPattern = BITS_001;
        break;
      }
      if (!b0 && b1 && !b2) {
        thisNode->bitPattern = BITS_010;
        break;
      }
      if (!b0 && b1 && b2) {
        thisNode->bitPattern = BITS_011;
        break;
      }
      if (b0 && !b1 && !b2) {
        thisNode->bitPattern = BITS_100;
        break;
      }
      if (b0 && !b1 && b2) {
        thisNode->bitPattern = BITS_101;
        break;
      }
      if (b0 && b1 && !b2) {
        thisNode->bitPattern = BITS_110;
        break;
      }
    case 2:
      thisNode->patternLength = 2;
      b0 = (bs[0 + offset] != 0);
      b1 = (bs[1 + offset] != 0);
      if (!b0 && !b1) {
        thisNode->bitPattern = BITS_00;
        break;
      }
      if (!b0 && b1) {
        thisNode->bitPattern = BITS_01;
        break;
      }
      if (b0 && !b1) {
        thisNode->bitPattern = BITS_10;
        break;
      }
    case 1:
      thisNode->patternLength = 1;
      b0 = (bs[0 + offset] != 0);
      if (!b0) {
        thisNode->bitPattern = BITS_0;
        break;
      }
      else {
        thisNode->patternLength = 0;
      }
  }

  if (thisNode->patternLength == 0) {
    return (False);
  }
  else {
    return (True);
  }
}

