#include <iostream>
#include "Curl.hpp"

int main() {
    Curl<CurlMode::GET, CurlResonse::BODY, CurlBinarySwitch::DISABLE> t;
}
