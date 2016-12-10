// Copyright Hugh Perkins 2015 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "luacpptemplater_gtest_supp.h"

#include "LuaCppTemplater/LuaCppTemplater.h"

using namespace std;

TEST(testLuaCppTemplater, basicsubstitution1) {
    string source = "This is my {{avalue}} template.";

    LuaCppTemplater mytemplate;
    mytemplate.set("avalue", 3);
    string result = mytemplate.render(source);
    cout << result << endl;
    string expectedResult = "This is my 3 template.";
    EXPECT_EQ(expectedResult, result);
    string result2 = mytemplate.render(source);
    EXPECT_EQ(expectedResult, result2);
}
TEST(testLuaCppTemplater, basicsubstitution1b) {
    string source = "This is my {{avalue}} template";

    LuaCppTemplater mytemplate;
    mytemplate.set("avalue", 3);
    string result = mytemplate.render(source);
    cout << result << endl;
    string expectedResult = "This is my 3 template";
    EXPECT_EQ(expectedResult, result);
    string result2 = mytemplate.render(source);
    EXPECT_EQ(expectedResult, result2);
}
TEST(testLuaCppTemplater, basicsubstitution) {
    string source = "\n"
"        This is my {{avalue}} template.  It's {{secondvalue}}...\n"
"        Today's weather is {{weather}}.\n"
"    \n";

    LuaCppTemplater mytemplate;
    mytemplate.set("avalue", 3);
    mytemplate.set("secondvalue", 123);
    mytemplate.set("weather", "rain");
    string result = mytemplate.render(source);
    cout << result << endl;
    string expectedResult = "\n"
"        This is my 3 template.  It's 123...\n"
"        Today's weather is rain.\n"
"    \n";
    EXPECT_EQ(expectedResult, result);
    string result2 = mytemplate.render(source);
    EXPECT_EQ(expectedResult, result2);
}
TEST(testLuaCppTemplater, startofsection) {
    string source = "{{avalue}} template";

    LuaCppTemplater mytemplate;
    mytemplate.set("avalue", 3);
    string result = mytemplate.render(source);
    cout << result << endl;
    string expectedResult = "3 template";
    EXPECT_EQ(expectedResult, result);
}
TEST(testLuaCppTemplater, endofsection) {
    string source = "template {{avalue}}";

    LuaCppTemplater mytemplate;
    mytemplate.set("avalue", 3);
    string result = mytemplate.render(source);
    cout << result << endl;
    string expectedResult = "template 3";
    EXPECT_EQ(expectedResult, result);
}
TEST(testLuaCppTemplater, loop) {
    string source = "\n"
"        {% for i=0,its-1 do %}\n"
"            a[{{i}}] = image[{{i}}];\n"
"        {% end %}\n"
"    \n";

    LuaCppTemplater mytemplate;
    mytemplate.set("its", 3);
    string result = mytemplate.render(source);
    cout << result << endl;
    string expectedResult = "\n"
"        \n"
"            a[0] = image[0];\n"
"        \n"
"            a[1] = image[1];\n"
"        \n"
"            a[2] = image[2];\n"
"        \n"
"    \n";
    EXPECT_EQ(expectedResult, result);
}

TEST(testLuaCppTemplater, nestedloop) {
    string source = "\n"
"{% for i=0,its-1 do %}a[{{i}}] = image[{{i}}];\n"
"{% for j=0,1 do %}b[{{j}}] = image[{{j}}];\n"
"{% end %}{% end %}\n"
"";

    LuaCppTemplater mytemplate;
    mytemplate.set("its", 3);
    string result = mytemplate.render(source);
    cout << "[" << result << "]" << endl;
    string expectedResult = "\n"
"a[0] = image[0];\n"
"b[0] = image[0];\n"
"b[1] = image[1];\n"
"a[1] = image[1];\n"
"b[0] = image[0];\n"
"b[1] = image[1];\n"
"a[2] = image[2];\n"
"b[0] = image[0];\n"
"b[1] = image[1];\n"
"\n"
"";
    EXPECT_EQ(expectedResult, result);
}

TEST(testLuaCppTemplater, foreachloop) {
    string source = "\n"
        "{% for _,name in ipairs(names) do %}{{name}}\n"
        "{% end %}\n"
        "";
    LuaCppTemplater mytemplate;
//    cout << mytemplate << endl;
    vector<string> values;
    values.push_back("blue");
    values.push_back("green");
    values.push_back("red");
    mytemplate.set("names", values);
    string result = mytemplate.render(source);
    cout << "[" << result << "]" << endl;
    string expectedResult = "\n"
"blue\n"
"green\n"
"red\n"
"\n";
    EXPECT_EQ(expectedResult, result);
}

TEST(testLuaCppTemplater, codesection) {
    string source = "\n"
        "{%\n"
        "sum=0\n"
        "for i=1,3 do\n"
        "  if i <= 2 then \n"
        "   sum = sum + 1\n"
        "  end\n"
        "end\n"
        "%}\n"
        "{{sum}}\n"
        "";
    LuaCppTemplater mytemplate;
    string result = mytemplate.render(source);
    cout << "[" << result << "]" << endl;
    string expectedResult = "\n\n2\n";
    EXPECT_EQ(expectedResult, result);
}

TEST(testLuaCppTemplater, codingerror) {
    string source = "\n"
        "{%\n"
        "sum=foo.blah\n"
        "%}\n"
        "";
    LuaCppTemplater mytemplate;
    EXPECT_THROW(mytemplate.render(source), std::runtime_error);
}

TEST(testLuaCppTemplater, include) {
    string source = "\n"
        "{{include_tensorinfocl}}\n"
        "\n"
        "";
    string source2 = "var color = '{{color}}';\n"
      "var {{foo}} = 0;\n";
    LuaCppTemplater mytemplate;
    mytemplate.set("include_tensorinfocl", source2);
    mytemplate.set("color", "blue");
    mytemplate.set("foo", "blah");
    string result = mytemplate.render(source);
    cout << "[" << result << "]" << endl;
    string expectedResult = "\nvar color = 'blue';\nvar blah = 0;\n\n\n";
    EXPECT_EQ(expectedResult, result);
}

TEST(testLuaCppTemplater, function) {
    string source = "\n"
        "{% function foo() %}\n"
        "this is text inside foo function\n"
        "{% end %}\n"
        "some text\n"
        "{% foo() %}\n"
        "------\n"
        "{% foo() %}\n"
        "more text\n"
        "\n"
        "";
    LuaCppTemplater mytemplate;
    string result = mytemplate.render(source);
    cout << "[" << result << "]" << endl;
    string expectedResult =
        "\n"
        "\n"
        "some text\n"
        "\n"
        "this is text inside foo function\n"
        "\n"
        "------\n"
        "\n"
        "this is text inside foo function\n"
        "\n"
        "more text\n"
        "\n";
    EXPECT_EQ(expectedResult, result);
}
