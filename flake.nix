{
  description = "advent";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/master";
  inputs.foolnotion.url = "github:foolnotion/nur-pkg";

  outputs = { self, flake-utils, nixpkgs, foolnotion }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ foolnotion.overlay ];
          };
          llvm = pkgs.llvmPackages_14;
          stdenv = pkgs.overrideCC llvm.stdenv (
            llvm.clang.override { gccForLibs = pkgs.gcc12.cc; }
          );
        in rec
        {
          devShells.default = stdenv.mkDerivation {
            name = "aoc-2021";
            hardeningDisable = [ "all" ];
            impureUseNativeOptimizations = true;
            nativeBuildInputs = with pkgs; [ cmake-init cmake clang-tools cppcheck ];
            buildInputs = with pkgs; [
                # python environment for bindings and scripting
                cmake-language-server
                cpp-lazy
                doctest
                eigen
                fmt
                gdb
                hotspot
                hyperfine
                linuxPackages_latest.perf
                mdspan
                openlibm
                pkg-config
                robin-hood-hashing
                scnlib
                seer
                valgrind
                xxHash
                openssl
              ];

              shellHook = ''
                alias bb="cmake --build build -j"
              '';
          };
        }
      );
}
