/*
 * EndPoint_test.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "EndPoint.h"

#include "RingBufferWriter.h"
#include "RingBufferReader.h"
#include "FrameBufferUserFrameHandler.h"

#include <iostream>

namespace hdlc {

enum station_t {
	A, B
};

station_t other(station_t station) {
	return station == A ? B : A;
}

BOOST_AUTO_TEST_CASE( test_EndPoint_does_nothing_while_disconnected ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter sinkA = RingBufferWriter(mediumAB);
	RingBufferWriter sinkB = RingBufferWriter(mediumBA);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	outgoingFrameBuffer[A].put(0x42);
	outgoingFrameBuffer[A].endFrame();

	for(int i = 0; i < 100; i++) {
		endPoint[A].schedule();
		endPoint[B].schedule();
	}

	BOOST_CHECK(0 == incomingFrameBuffer[B].size());
}

BOOST_AUTO_TEST_CASE( test_EndPoint_does_nothing_when_give_no_frames ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter sinkA = RingBufferWriter(mediumAB);
	RingBufferWriter sinkB = RingBufferWriter(mediumBA);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	endPoint[A].connect();
	endPoint[B].connect();

	for(int i = 0; i < 100; i++) {
		endPoint[A].schedule();
		endPoint[B].schedule();
	}

	BOOST_CHECK(0 == incomingFrameBuffer[A].size());
	BOOST_CHECK(0 == incomingFrameBuffer[B].size());
}

BOOST_AUTO_TEST_CASE( test_EndPoint_moves_one_frame ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter sinkA = RingBufferWriter(mediumAB);
	RingBufferWriter sinkB = RingBufferWriter(mediumBA);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	endPoint[A].connect();
	endPoint[B].connect();

	outgoingFrameBuffer[A].put(0x42);
	outgoingFrameBuffer[A].endFrame();

	for(int i = 0; i < 100; i++) {
		endPoint[A].schedule();
		endPoint[B].schedule();
	}

	BOOST_CHECK(0 == outgoingFrameBuffer[A].size());
	BOOST_CHECK(1 == incomingFrameBuffer[B].size());
	BOOST_CHECK(1 == incomingFrameBuffer[B][0].size());
	BOOST_CHECK(0x42 == incomingFrameBuffer[B][0][0]);
}

BOOST_AUTO_TEST_CASE( test_EndPoint_moves_seven_frames ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter sinkA = RingBufferWriter(mediumAB);
	RingBufferWriter sinkB = RingBufferWriter(mediumBA);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	endPoint[A].connect();
	endPoint[B].connect();

	int frameCount = 2;
	for(int i = 0; i < frameCount; i++) {
		outgoingFrameBuffer[A].put(0x42 + i);
		outgoingFrameBuffer[A].endFrame();
	}

	for(int i = 0; i < 200; i++) {
		endPoint[A].schedule();
		endPoint[B].schedule();
	}

	BOOST_CHECK(0 == outgoingFrameBuffer[A].size());
	BOOST_CHECK(frameCount == incomingFrameBuffer[B].size());
	for(int i = 0; i < frameCount; i++) {
		BOOST_CHECK(1 == incomingFrameBuffer[B][0].size());
		BOOST_CHECK((0x42 + i) == incomingFrameBuffer[B][0][0]);
		incomingFrameBuffer[B].removeFrame();
	}
}

BOOST_AUTO_TEST_CASE( test_EndPoint_moves_one_frame_in_each_direction ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter sinkA = RingBufferWriter(mediumAB);
	RingBufferWriter sinkB = RingBufferWriter(mediumBA);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	endPoint[A].connect();
	endPoint[B].connect();

	outgoingFrameBuffer[A].put(0x42);
	outgoingFrameBuffer[A].endFrame();

	outgoingFrameBuffer[B].put(0x43);
	outgoingFrameBuffer[B].endFrame();

	for(int i = 0; i < 100; i++) {
		endPoint[A].schedule();
		endPoint[B].schedule();
	}

	BOOST_CHECK(0 == outgoingFrameBuffer[A].size());
	BOOST_CHECK(0 == outgoingFrameBuffer[B].size());

	BOOST_CHECK(1 == incomingFrameBuffer[B].size());
	BOOST_CHECK(1 == incomingFrameBuffer[B][0].size());
	BOOST_CHECK(0x42 == incomingFrameBuffer[B][0][0]);

	BOOST_CHECK(1 == incomingFrameBuffer[A].size());
	BOOST_CHECK(1 == incomingFrameBuffer[A][0].size());
	BOOST_CHECK(0x43 == incomingFrameBuffer[A][0][0]);
}

BOOST_AUTO_TEST_CASE( test_EndPoint_ignores_frames_with_unexpected_sequence_number ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter sinkA = RingBufferWriter(mediumAB);
	RingBufferWriter sinkB = RingBufferWriter(mediumBA);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	endPoint[A].connect();
	endPoint[B].connect();

	mediumAB.put(EscapingSource::FLAG);	// flag
	uint16_t crc = 0xFFFF;
	mediumAB.put(0x00 ^ 0x01);			// unexpected sequence number
	crc_ccitt_update(crc, 0x00 ^ 0x01);
	mediumAB.put(0x42);		 			// data
	crc_ccitt_update(crc, 0x42);
	mediumAB.put(crc >> 8);				// crc msb
	mediumAB.put(crc & 0xFF);				// crc lsb
	mediumAB.put(EscapingSource::FLAG);	// flag

	while(!mediumAB.isEmpty()) {
		endPoint[B].schedule();
	}

	BOOST_CHECK(incomingFrameBuffer[B].size() == 0);
}

class NoisySink : public Sink<uint8_t> {
	Sink<uint8_t>& sink;

public:
	NoisySink(Sink<uint8_t>& sink) : sink(sink) {}

	virtual ~NoisySink() {}

	virtual bool isReady() { return sink.isReady(); }
	virtual void write(const uint8_t& octet) {
		if(rand() & 0xFF) {
			sink.write(octet);
		} else {
			uint8_t noise = rand();
			if(noise != 0) {
				sink.write(octet ^ noise);
			}
		}
	}
};

BOOST_AUTO_TEST_CASE( test_EndPoint_moves_one_hundred_frames_in_each_direction_with_noise ) {
	RingBuffer<64> mediumAB;
	RingBuffer<64> mediumBA;

	RingBufferWriter writerA = RingBufferWriter(mediumAB);
	NoisySink sinkA = NoisySink(writerA);
	RingBufferWriter writerB = RingBufferWriter(mediumBA);
	NoisySink sinkB = NoisySink(writerB);
	RingBufferReader readerA = RingBufferReader(mediumBA);
	RingBufferReader readerB = RingBufferReader(mediumAB);

	EscapingSink sink[2] = {
			EscapingSink(sinkA),
			EscapingSink(sinkB)
	};

	EscapingSource source[2] = {
			EscapingSource(readerA),
			EscapingSource(readerB)
	};

	FrameBuffer outgoingFrameBuffer[2];
	FrameBuffer incomingFrameBuffer[2];

	FrameTransmitter frameTransmitter[2] = {
			FrameTransmitter(sink[A]),
			FrameTransmitter(sink[B])
	};

	FrameBufferUserFrameHandler userFrameHandler[2] = {
			FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
			FrameBufferUserFrameHandler(incomingFrameBuffer[B])
	};

	FrameReceiver frameReceiver[2] = {
			FrameReceiver(source[A]),
			FrameReceiver(source[B])
	};

	EndPoint endPoint[2] = {
			EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
			EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
	};

	frameReceiver[A].setFrameHandler(&(endPoint[A]));
	frameReceiver[B].setFrameHandler(&(endPoint[B]));

	endPoint[A].connect();
	endPoint[B].connect();

	int framesSent[2] = { 0, 0 };
	int framesReceived[2] = { 0, 0 };

	int count = 0;
	while(framesReceived[A] != 100 || framesReceived[B] != 100) {
		count++;
		for(int i = 0; i < 2; i++) {
			while(!outgoingFrameBuffer[i].isFull() && framesSent[i] < 100) {
				outgoingFrameBuffer[i].put(framesSent[i]++);
				outgoingFrameBuffer[i].endFrame();
			}
		}
		endPoint[A].schedule();
		endPoint[B].schedule();

		for(int i = 0; i < 2; i++) {
			while(!incomingFrameBuffer[i].isEmpty()) {
				BOOST_CHECK(1 == incomingFrameBuffer[i][0].size());
				BOOST_CHECK(framesReceived[i]++ == incomingFrameBuffer[i][0][0]);
				incomingFrameBuffer[i].removeFrame();
			}
		}
	}
	std::cout << count << std::endl;
}

} /* namespace hdlc */
