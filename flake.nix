{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    systems.url = "github:nix-systems/default";
  };

  outputs = inputs: inputs.flake-parts.lib.mkFlake { inherit inputs; } {
    systems = import inputs.systems;
    perSystem = { config, self', inputs', pkgs, system, ... }: let
      pkgs = import inputs.nixpkgs {
        inherit system;
      };
    in {
      _module.args.pkgs = import inputs.nixpkgs {
        config.allowUnfree = true;
      };
      devShells.default = pkgs.mkShell rec {
        buildInputs = with pkgs; [
          cmake
          ninja
          llvmPackages_14.clang-tools
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
        LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath buildInputs}";
        LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
      };
    };
  };
}
