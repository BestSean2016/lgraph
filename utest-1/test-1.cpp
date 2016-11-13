#include <gtest/gtest.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  // if (argc != 3) {
  //   printf("Usage testing <json config file> <json config file2>\n");
  //   return (0);
  // }

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

#include "../attr-1/chinese.h"

TEST(convert, utf8_to_ucs2) {
  char hello[100] = "大家好！";
  char temp[100];
  
  EXPECT_EQ(0, convert(hello, temp, 33, 100, "UTF-8", "UCS-2"));

  // dumpbuf("hello", hello, 100);
  // dumpbuf("temp", temp, 100);
  unsigned char t1[10] = {0x27, 0x59, 0xB6, 0x5B, 0x7D, 0x59, 0x01, 0xFF, 0x00, 0x00};
  EXPECT_EQ(memcmp(temp, t1, 10), 0);
}


TEST(convert, utf8_to_ucs4) {
    char hello[100] = "大家好！";
    char temp[100];
  EXPECT_EQ(0, convert(hello, temp, 33, 100, "UTF-8", "UCS-4"));

  // dumpbuf("hello", hello, 100);
  // dumpbuf("temp", temp, 100);
  unsigned char t2[20] = {0x00, 0x00, 0x59, 0x27, 0x00, 0x00, 0x5B, 0xB6,
                          0x00, 0x00, 0x59, 0x7D, 0x00, 0x00, 0xFF, 0x01,
                          0x00, 0x00, 0x00, 0x00};
  EXPECT_EQ(memcmp(temp, t2, 20), 0);


  // for (int k = 1; k <= 100; k++)
  //     for (int j = 1; j <= 100; j++) {
  //         int ret = convert(hello, temp, k, j, "UTF-8", "UCS-4");
  //         if (ret == 0) printf("%d %d %d\n", k, j, ret);
  //     }
}
