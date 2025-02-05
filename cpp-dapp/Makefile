CXX  := g++


.PHONY: clean 3rdparty

dapp: dapp.cpp uint256_t/uint256_t.cpp uint256_t/uint128_t/uint128_t.cpp
	make -C 3rdparty
	$(CXX) -pthread -std=c++11 -o $@ $^

clean:
	@rm -rf dapp
	make -C 3rdparty clean
