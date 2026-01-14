#include "protocol.h"
#include <QtEndian>
#include <cstring>

std::optional<UserData> Protocol::parse(const QByteArray& buffer) {
    if (buffer.size() < 8) return std::nullopt;

    const uchar* ptr = reinterpret_cast<const uchar*>(buffer.constData());
    size_t offset = 0;

    if (offset + 4 > static_cast<size_t>(buffer.size())) return std::nullopt;
    uint32_t userId = qFromBigEndian<quint32>(ptr + offset);
    offset += 4;

    if (offset + 2 > static_cast<size_t>(buffer.size())) return std::nullopt;
    quint16 nameLen = qFromBigEndian<quint16>(ptr + offset);
    offset += 2;

    if (nameLen > MAX_NAME_LEN || offset + nameLen > static_cast<size_t>(buffer.size()))
        return std::nullopt;

    std::string name(reinterpret_cast<const char*>(ptr + offset), nameLen);
    offset += nameLen;

    if (offset + 2 > static_cast<size_t>(buffer.size())) return std::nullopt;
    quint16 emailLen = qFromBigEndian<quint16>(ptr + offset);
    offset += 2;

    if (emailLen > MAX_EMAIL_LEN || offset + emailLen != static_cast<size_t>(buffer.size()))
        return std::nullopt;

    std::string email(reinterpret_cast<const char*>(ptr + offset), emailLen);
    return UserData{userId, name, email};
}

QByteArray Protocol::serialize(const UserData& data) {
    if (data.name.size() > MAX_NAME_LEN || data.email.size() > MAX_EMAIL_LEN) {
        return {};
    }

    quint16 nameLen = static_cast<quint16>(data.name.size());
    quint16 emailLen = static_cast<quint16>(data.email.size());
    quint32 payloadSize = 4 + 2 + nameLen + 2 + emailLen;

    QByteArray buf;
    buf.resize(4 + payloadSize);

    uchar* ptr = reinterpret_cast<uchar*>(buf.data());
    qToBigEndian(payloadSize, ptr); ptr += 4;
    qToBigEndian(data.userId, ptr); ptr += 4;
    qToBigEndian(nameLen, ptr); ptr += 2;
    std::memcpy(ptr, data.name.data(), nameLen); ptr += nameLen;
    qToBigEndian(emailLen, ptr); ptr += 2;
    std::memcpy(ptr, data.email.data(), emailLen);

    return buf;
}