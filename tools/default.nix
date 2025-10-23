pkgs: rec {
  version = "v1.2.3";
  cacheUrl = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/${version}";

  x86_64-linux = pkgs.callPackage ./tools.nix {
    url = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/${version}/OdysseyDecomp-binaries_x86_64-Linux.tar.xz";
    sha256 = "sha256-tbWE87uVO7fDP9PeVzzZ8aAPIZky18WLFsKsDuz4Md4=";
    inherit version;
  };
  aarch64-linux = pkgs.callPackage ./tools {
    url = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/${version}/OdysseyDecomp-binaries_aarch64-Linux.tar.xz";
    sha256 = "sha256-C0tESPXtEN82NhxRoJuC+Jt5CCJlgHMra8FwY9T0NUA=";
    inherit version;
  };
  aarch64-darwin = pkgs.callPackage ./tools {
    url = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/${version}/OdysseyDecomp-binaries_arm64-Darwin.tar.xz";
    sha256 = "sha256-vxoPpHNsSLCOOJtqZksrg1qLqronEbhYRt3GZ7+ukI8=";
    inherit version;
  };
}
