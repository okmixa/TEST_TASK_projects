#include <QtTest>
#include "../common/protocol.h"
#include "../common/validator.h"

class TestProtocol : public QObject {
    Q_OBJECT

private slots:
    void testValidSerialization() {
        struct TestCase {
            uint32_t id;
            std::string name;
            std::string email;
        } cases[] = {
            {1001, "Alice", "alice@example.com"},
            {2002, "Bob", "bob@test.org"},
            {3, "X", "y@z.co"}
        };

        for (const auto& tc : cases) {
            UserData data{tc.id, tc.name, tc.email};
            QByteArray packet = Protocol::serialize(data);
            QVERIFY2(!packet.isEmpty(), "Serialization failed");

            quint32 payloadSize = 4 + 2 + static_cast<quint16>(tc.name.size())
                                    + 2 + static_cast<quint16>(tc.email.size());
            QCOMPARE(static_cast<quint32>(packet.size()), 4 + payloadSize);

            auto parsed = Protocol::parse(packet.mid(4));
            QVERIFY2(parsed.has_value(), "Parse failed");
            QCOMPARE(parsed->userId, tc.id);
            QCOMPARE(parsed->name, tc.name);
            QCOMPARE(parsed->email, tc.email);
        }
    }

    void testInvalidEmail_rejectedByValidator() {
        QVERIFY(!Validator::isValidEmail("invalid"));
        QVERIFY(!Validator::isValidEmail("@example.com"));
        QVERIFY(!Validator::isValidEmail("user@"));
        QVERIFY(Validator::isValidEmail("user+tag@example.co.uk"));
    }

    void testEdgeLengths() {
        std::string maxName(255, 'x');
        std::string maxEmail(255, 'y');
        UserData data{999, maxName, maxEmail};
        QByteArray packet = Protocol::serialize(data);
        QVERIFY(!packet.isEmpty());

        auto parsed = Protocol::parse(packet.mid(4));
        QVERIFY(parsed.has_value());
        QCOMPARE(parsed->name.size(), size_t(255));
        QCOMPARE(parsed->email.size(), size_t(255));
    }

    void testOversizedRejected() {
        std::string tooLong(256, 'x');
        UserData bad1{1, tooLong, "a@b.co"};
        QVERIFY(Protocol::serialize(bad1).isEmpty());

        UserData bad2{1, "ok", tooLong};
        QVERIFY(Protocol::serialize(bad2).isEmpty());
    }

    void benchmarkSerializeParse() {
        QBENCHMARK {
            UserData data{123, "Test User", "test@example.org"};
            QByteArray packet = Protocol::serialize(data);
            auto result = Protocol::parse(packet.mid(4));
            QVERIFY(result.has_value());
        }
    }
};

QTEST_MAIN(TestProtocol)
#include "tst_protocol.moc"