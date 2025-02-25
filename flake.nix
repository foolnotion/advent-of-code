{
  description = "advent";

  inputs.flake-parts.url = "github:hercules-ci/flake-parts";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/master";
  inputs.foolnotion.url = "github:foolnotion/nur-pkg";
  inputs.pratt.url = "github:foolnotion/pratt-parser-calculator";

  inputs.pratt.inputs.nixpkgs.follows = "nixpkgs";
  inputs.foolnotion.inputs.nixpkgs.follows = "nixpkgs";

  outputs = inputs@{ self, flake-parts, nixpkgs, foolnotion, pratt }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" "x86_64-darwin" "aarch64-darwin" ];

      perSystem = { pkgs, system, ... }:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ foolnotion.overlay ];
          };
          stdenv = pkgs.llvmPackages_19.stdenv;
          #stdenv = pkgs.gcc14Stdenv;
        in
        rec {
          devShells.default = stdenv.mkDerivation {
            name = "aoc";
            hardeningDisable = [ "all" ];
            impureUseNativeOptimizations = true;
            nativeBuildInputs = with pkgs; [ cmake-init cmake clang-tools_19 cppcheck ninja ];
            buildInputs = with pkgs; [
              bfg-repo-cleaner
              boost
              cmake-language-server
              cpp-flux
              cpp-lazy
              cpp-sort
              cpptrace
              doctest
              dynamic-bitset
              eigen
              fast-float
              fastor
              fmt_11
              fplus
              gch-small-vector
              gdb
              gtl
              hotspot
              howard-hinnant-date
              hyperfine
              libassert
              libdwarf
              linuxPackages_latest.perf
              mdspan
              nanobench
              nlohmann_json
              pkg-config
              pratt.packages.${system}.default
              scnlib
              seer
              tlx
              unordered_dense
              valgrind
              xxhash_cpp
              z3_4_12
              zstd
            ];

            shellHook = ''
              alias bb="cmake --build build -j"
            '';

            env = {
              CLANGD_FLAGS = "--query-driver=${pkgs.lib.getExe stdenv.cc}";
            };
          };
        };
    };
}
