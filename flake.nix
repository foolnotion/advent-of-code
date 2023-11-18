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
          stdenv_ = pkgs.llvmPackages_16.stdenv;
        in rec
        {
          devShells.default = stdenv_.mkDerivation {
            name = "aoc";
            hardeningDisable = [ "all" ];
            impureUseNativeOptimizations = true;
            nativeBuildInputs = with pkgs; [ cmake-init cmake clang-tools_16 cppcheck ];
            buildInputs = with pkgs; [
                cmake-language-server
                cpp-lazy
                cpp-sort
                doctest
                eigen
                fast_float
                fmt
                gdb
                hotspot
                hyperfine
                linuxPackages_latest.perf
                mdspan
                nlohmann_json
                openlibm
                pkg-config
                pratt-parser
                robin-hood-hashing
                scnlib
                seer
                unordered_dense
                valgrind
              ];

              shellHook = ''
                alias bb="cmake --build build -j"
              '';
          };
        }
      );
}
