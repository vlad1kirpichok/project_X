#ifndef CODE_CAVE_PAYLOAD_H
#define CODE_CAVE_PAYLOAD_H

#ifdef BUILD32

unsigned char codeCavePayload32[] =
    "\xeb\x14\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\x59\xba\x0f\x00\x00\x00\xcd\x80\xeb\x14\xe8\xe7\xff\xff\xff\x48\x65\x6c\x6c\x6f\x2c\x20\x57\x6f\x72\x6c\x64\x21\x0d\x0a";

#elif defined(BUILD64)

unsigned char codeCavePayload64[] =
    "\xeb\x19\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\x59\xba\x0f\x00\x00\x00\xcd\x80\xba\x00\x00\x00\x00\xeb\x14\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x2c\x20\x4d\x53\x55\x21\x21\x21\x0d\x0a";
#endif

#endif
