#include "../../CK2ToEU4/Source/Mappers/RegionMapper/Area.h"
#include "../../CK2ToEU4/Source/Mappers/RegionMapper/Region.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_RegionTests, blankRegionLoadsWithNoAreas)
{
	std::stringstream input;
	const mappers::Region region(input);

	ASSERT_TRUE(region.getAreas().empty());
}

TEST(Mappers_RegionTests, areaCanBeLoaded)
{
	std::stringstream input;
	input << "areas = { test_area } \n";
	const mappers::Region region(input);

	ASSERT_FALSE(region.getAreas().empty());
	ASSERT_EQ(region.getAreas().find("test_area")->first, "test_area");
}

TEST(Mappers_RegionTests, multipleAreasCanBeLoaded)
{
	std::stringstream input;
	input << "areas = { test_area area2 area3 } \n";
	const mappers::Region region(input);

	ASSERT_EQ(region.getAreas().size(), 3);
}

TEST(Mappers_RegionTests, regionCanBeLinkedToArea)
{
	std::stringstream input;
	input << "areas = { test_area area2 area3 } \n";
	mappers::Region region(input);

	std::stringstream input2;
	input << "{ 3 4 5 6 } \n";
	auto area2 = std::make_shared<mappers::Area>(input);

	ASSERT_FALSE(region.getAreas().find("area2")->second);
	region.linkArea(std::pair("area2", area2));
	ASSERT_TRUE(region.getAreas().find("area2")->second);
}

TEST(Mappers_RegionTests, LinkedRegionCanLocateProvince)
{
	std::stringstream input;
	input << "areas = { area2 } \n";
	mappers::Region region(input);

	std::stringstream input2;
	input2 << "{ 3 4 5 6 } \n";
	auto area2 = std::make_shared<mappers::Area>(input2);

	region.linkArea(std::pair("area2", area2));

	ASSERT_TRUE(region.regionContainsProvince(5));
}

TEST(Mappers_RegionTests, LinkedRegionWillFailForProvinceMismatch)
{
	std::stringstream input;
	input << "areas = { area2 } \n";
	mappers::Region region(input);

	std::stringstream input2;
	input << "{ 3 4 5 6 } \n";
	auto area2 = std::make_shared<mappers::Area>(input);

	region.linkArea(std::pair("area2", area2));

	ASSERT_FALSE(region.regionContainsProvince(9));
}
