#include "catch.hpp"
#include <mdcmTag.h>
#include <mdcmTransferSyntax.h>
#include <mdcmMediaStorage.h>
#include <mdcmPixelFormat.h>
#include <mdcmVR.h>
#include <mdcmByteValue.h>

// Tag creation and accessors
TEST_CASE("Tag creation from group and element", "[dicom]")
{
	mdcm::Tag t(0x0010, 0x0010);
	REQUIRE(t.GetGroup() == 0x0010);
	REQUIRE(t.GetElement() == 0x0010);
}

TEST_CASE("Tag comparison operators", "[dicom]")
{
	mdcm::Tag a(0x0008, 0x0010);
	mdcm::Tag b(0x0010, 0x0010);
	REQUIRE(a < b);
	REQUIRE(a != b);
	REQUIRE(a == mdcm::Tag(0x0008, 0x0010));
}

TEST_CASE("Tag public vs private", "[dicom]")
{
	mdcm::Tag pub(0x0010, 0x0010);
	mdcm::Tag priv(0x0011, 0x0010);
	REQUIRE(pub.IsPublic());
	REQUIRE(priv.IsPrivate());
}

// VR handling
TEST_CASE("VR string round-trip", "[dicom]")
{
	const char * s = mdcm::VR::GetVRString(mdcm::VR::CS);
	REQUIRE(s != nullptr);
	REQUIRE(std::string(s) == "CS");
	REQUIRE(mdcm::VR::GetVRType("CS") == mdcm::VR::CS);
}

TEST_CASE("VR binary vs ASCII classification", "[dicom]")
{
	REQUIRE(mdcm::VR::IsASCII(mdcm::VR::LO));
	REQUIRE(mdcm::VR::IsBinary(mdcm::VR::OW));
	REQUIRE_FALSE(mdcm::VR::IsASCII(mdcm::VR::OW));
	REQUIRE_FALSE(mdcm::VR::IsBinary(mdcm::VR::LO));
}

// TransferSyntax enum completeness
TEST_CASE("TransferSyntax known values are valid", "[dicom]")
{
	mdcm::TransferSyntax ts_ile(mdcm::TransferSyntax::ImplicitVRLittleEndian);
	REQUIRE(ts_ile.IsValid());
	REQUIRE(ts_ile.IsImplicit());

	mdcm::TransferSyntax ts_ele(mdcm::TransferSyntax::ExplicitVRLittleEndian);
	REQUIRE(ts_ele.IsValid());
	REQUIRE(ts_ele.IsExplicit());

	mdcm::TransferSyntax ts_j2k(mdcm::TransferSyntax::JPEG2000Lossless);
	REQUIRE(ts_j2k.IsValid());
	REQUIRE(ts_j2k.IsEncapsulated());
}

TEST_CASE("TransferSyntax TS_END is invalid", "[dicom]")
{
	mdcm::TransferSyntax ts(mdcm::TransferSyntax::TS_END);
	REQUIRE_FALSE(ts.IsValid());
}

TEST_CASE("TransferSyntax string lookup", "[dicom]")
{
	const char * s = mdcm::TransferSyntax::GetTSString(
		mdcm::TransferSyntax::ExplicitVRLittleEndian);
	REQUIRE(s != nullptr);
	REQUIRE(std::string(s) == "1.2.840.10008.1.2.1");
}

// MediaStorage enum completeness
TEST_CASE("MediaStorage known types have strings", "[dicom]")
{
	const char * ct = mdcm::MediaStorage::GetMSString(
		mdcm::MediaStorage::CTImageStorage);
	REQUIRE(ct != nullptr);

	const char * mr = mdcm::MediaStorage::GetMSString(
		mdcm::MediaStorage::MRImageStorage);
	REQUIRE(mr != nullptr);

	const char * sc = mdcm::MediaStorage::GetMSString(
		mdcm::MediaStorage::SecondaryCaptureImageStorage);
	REQUIRE(sc != nullptr);
}

TEST_CASE("MediaStorage MS_END is undefined", "[dicom]")
{
	mdcm::MediaStorage ms;
	REQUIRE(ms.IsUndefined());
}

TEST_CASE("MediaStorage CT is an image type", "[dicom]")
{
	REQUIRE(mdcm::MediaStorage::IsImage(mdcm::MediaStorage::CTImageStorage));
	REQUIRE(mdcm::MediaStorage::IsImage(mdcm::MediaStorage::MRImageStorage));
}

// PixelFormat bit depth handling
TEST_CASE("PixelFormat 16-bit unsigned", "[dicom]")
{
	mdcm::PixelFormat pf(1, 16, 16, 15, 0);
	REQUIRE(pf.GetBitsAllocated() == 16);
	REQUIRE(pf.GetBitsStored() == 16);
	REQUIRE(pf.GetHighBit() == 15);
	REQUIRE(pf.GetPixelRepresentation() == 0);
	REQUIRE(pf.GetSamplesPerPixel() == 1);
}

TEST_CASE("PixelFormat 8-bit RGB", "[dicom]")
{
	mdcm::PixelFormat pf(3, 8, 8, 7, 0);
	REQUIRE(pf.GetSamplesPerPixel() == 3);
	REQUIRE(pf.GetBitsAllocated() == 8);
}

TEST_CASE("PixelFormat scalar type from enum", "[dicom]")
{
	mdcm::PixelFormat pf(mdcm::PixelFormat::UINT16);
	REQUIRE(pf.GetBitsAllocated() == 16);
	REQUIRE(pf.GetPixelRepresentation() == 0);
}

TEST_CASE("PixelFormat equality", "[dicom]")
{
	mdcm::PixelFormat a(1, 16, 12, 11, 0);
	mdcm::PixelFormat b(1, 16, 12, 11, 0);
	mdcm::PixelFormat c(1, 16, 16, 15, 0);
	REQUIRE(a == b);
	REQUIRE(a != c);
}

// Malformed tag handling: empty ByteValue
TEST_CASE("Empty ByteValue has zero length", "[dicom]")
{
	mdcm::ByteValue bv;
	const std::vector<char> & data = bv;
	REQUIRE(data.empty());
}

TEST_CASE("ByteValue from empty data", "[dicom]")
{
	std::vector<char> empty;
	mdcm::ByteValue bv(empty);
	const std::vector<char> & data = bv;
	REQUIRE(data.empty());
}
