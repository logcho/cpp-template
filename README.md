# cpp-template

## Overview
This is a custom C++ template for Cartesi that builds upon the default template, adding additional functions and hex utilities for improved usability and readability. It provides a more efficient way to handle hex-encoded payloads and simplifies common operations in Cartesi's Linux environment.

## Features
- Enhanced hex utilities for easier encoding/decoding.
- Additional helper functions for streamlined development.
- Improved readability and maintainability.

## Installation & Setup
### Prerequisites
- [Install Docker Desktop and Cartesi CLI](https://docs.cartesi.io/cartesi-rollups/1.5/quickstart/)

## Test
### ERC20 Deposits
```
cast send 0x92c6bca388e99d6b304f1af3c3cd749ff0b591e2 "approve(address,uint256)" 0x9c21aeb2093c32ddbc53eef24b873bdcd1ada1db 20 --mnemonic "test test test test test test test test test test test junk"