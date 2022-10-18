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
          stdenv = pkgs.gcc12Stdenv;
        in rec
        {
          devShells.default = stdenv.mkDerivation {
            name = "aoc-2021";
            hardeningDisable = [ "all" ];
            impureUseNativeOptimizations = true;
            nativeBuildInputs = with pkgs; [ bear cmake-init cmake clang_14 clang-tools cppcheck ];
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
              ];

              shellHook = ''
                alias bb="cmake --build build -j"
              '';
          };
        }
      );
}
