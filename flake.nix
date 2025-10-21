{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    systems.url = "github:nix-systems/default";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import inputs.systems;
      perSystem =
        {
          system,
          ...
        }:
        let
          pkgs = import inputs.nixpkgs {
            inherit system;
          };

          tools = import ./tools pkgs;
          cacheUrl = tools.cacheUrl;
          tools-artifact = tools.${system};

          buildInputs = with pkgs; [
            cmake
            ninja
            llvmPackages_18.clang-tools
            llvmPackages_18.bintools
            ccache
            pkg-config

            (python311.withPackages (python-pkgs: [
              python-pkgs.capstone
              python-pkgs.colorama
              python-pkgs.cxxfilt
              python-pkgs.pyelftools
              python-pkgs.watchdog
              python-pkgs.python-Levenshtein
              python-pkgs.toml
            ]))
            openssl
            ncurses5
            ncurses6
          ];
          env = {
            LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath buildInputs}";
            LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
            NX_DECOMP_TOOLS_NX2ELF = "${tools-artifact}/bin/nx2elf";
          };
        in
        {
          _module.args.pkgs = import inputs.nixpkgs {
            config.allowUnfree = true;
          };
          formatter = pkgs.nixfmt-tree;
          devShells.default = pkgs.mkShell (
            {
              inherit buildInputs;
            }
            // env
          );
          packages = {
            setup = pkgs.writeShellApplication {
              name = "setup";
              text = ''
                link() {
                  rm -f "$2"
                  ln -s "$1" "$2"
                }

                echo -n "${cacheUrl}" > toolchain/cache-version-url.txt
                mkdir -p toolchain/bin
                link ${tools-artifact}/bin/clang toolchain/bin/clang
                link ${tools-artifact}/bin/ld.lld toolchain/bin/ld.lld
                link ${tools-artifact}/bin/decompme tools/decompme
                link ${tools-artifact}/bin/check tools/check
                link ${tools-artifact}/bin/listsym tools/listsym

                python3 tools/setup.py "$@"
              '';
              runtimeInputs = buildInputs;
              runtimeEnv = env;
            };
          };
        };
    };
}
