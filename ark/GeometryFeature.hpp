/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __ark_GeometryFeature_hpp__
#define __ark_GeometryFeature_hpp__

#include <string>
#include <vector>
#include "ark/Point.hpp"
#include "ark/Attribute.hpp"

namespace ark
{

class GeometryFeature
{
    public:
        std::string id;

        int16_t    type;

        int32_t    npoints;

        int32_t    nattributes;

        int32_t    ndata;

        std::vector< ark::Point > points;

        std::vector< ark::Attribute > attributes;

        std::vector< int16_t > data;

    public:
        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void *buf, int offset, int maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline int getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to reqad while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void *buf, int offset, int maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "GeometryFeature"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int GeometryFeature::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int GeometryFeature::decode(const void *buf, int offset, int maxlen)
{
    int pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int GeometryFeature::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t GeometryFeature::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* GeometryFeature::getTypeName()
{
    return "GeometryFeature";
}

int GeometryFeature::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    char* id_cstr = (char*) this->id.c_str();
    tlen = __string_encode_array(buf, offset + pos, maxlen - pos, &id_cstr, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->npoints, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->nattributes, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->ndata, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < this->npoints; a0++) {
        tlen = this->points[a0]._encodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    for (int a0 = 0; a0 < this->nattributes; a0++) {
        tlen = this->attributes[a0]._encodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->ndata > 0) {
        tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->data[0], this->ndata);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int GeometryFeature::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    int32_t __id_len__;
    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &__id_len__, 1);
    if(tlen < 0) return tlen; else pos += tlen;
    if(__id_len__ > maxlen - pos) return -1;
    this->id.assign(((const char*)buf) + offset + pos, __id_len__ - 1);
    pos += __id_len__;

    tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->npoints, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->nattributes, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->ndata, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    this->points.resize(this->npoints);
    for (int a0 = 0; a0 < this->npoints; a0++) {
        tlen = this->points[a0]._decodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    this->attributes.resize(this->nattributes);
    for (int a0 = 0; a0 < this->nattributes; a0++) {
        tlen = this->attributes[a0]._decodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->ndata) {
        this->data.resize(this->ndata);
        tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->data[0], this->ndata);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int GeometryFeature::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->id.size() + 4 + 1;
    enc_size += __int16_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    for (int a0 = 0; a0 < this->npoints; a0++) {
        enc_size += this->points[a0]._getEncodedSizeNoHash();
    }
    for (int a0 = 0; a0 < this->nattributes; a0++) {
        enc_size += this->attributes[a0]._getEncodedSizeNoHash();
    }
    enc_size += __int16_t_encoded_array_size(NULL, this->ndata);
    return enc_size;
}

uint64_t GeometryFeature::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == GeometryFeature::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, (void*)GeometryFeature::getHash };

    uint64_t hash = 0xe27fd9c2a5f448d8LL +
         ark::Point::_computeHash(&cp) +
         ark::Attribute::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

}

#endif