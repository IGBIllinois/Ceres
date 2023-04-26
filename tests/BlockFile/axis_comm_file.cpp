

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/AxisCommunicationsSerializer.hpp>
#include <cbdf/AxisCommunicationsParser.hpp>


class cAxisCommunicationsTestParser : public cAxisCommunicationsParser
{
public:
	int		mId = 0;
	int		mFramesPerSec = 0;
	int		mImageWidth = 0;
	int		mImageHeight = 0;

protected:
	void onActiveCameraId(int id) override { mId = id; };
	void onFramesPerSecond(int frames_per_sec) override { mFramesPerSec = frames_per_sec; };
	void onBitmap(const cBitmapBuffer& buffer) override {};
	void onJPEG(const cJpegBuffer& buffer) override {};
	void onMpegFrame(const cMpegFrameBuffer& buffer) override {};
	void onImageSize(int width, int height) override { mImageWidth = width; mImageHeight = height; }
};


TEST_CASE("Basic image info tests", "[axis communications tests]")
{
	SECTION("Testing write/read of basic image info data...")
	{
		const char* TEST_FILENAME = "axis_comm_test.ceres";

		int id = 1;
		int frames_per_sec = 3;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cAxisCommunicationsSerializer axis(1024, &wrt);
			axis.setVersion(1, 0);

			axis.writeActiveCameraId(id);
			axis.writeFramesPerSecond(frames_per_sec);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cAxisCommunicationsTestParser axis;
			rd.attach(&axis);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(axis.mId == id);

			result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(axis.mFramesPerSec == frames_per_sec);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


