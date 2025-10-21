pkgs: {
  cacheUrl = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/v1.2.3";

  x86_64-linux = pkgs.callPackage ./tools.nix {
    url = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/v1.2.3/OdysseyDecomp-binaries_x86_64-Linux.tar.xz";
    sha256 = "sha256-tbWE87uVO7fDP9PeVzzZ8aAPIZky18WLFsKsDuz4Md4=";
  };
  aarch64-linux = pkgs.callPackage ./tools {
    url = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/v1.2.3/OdysseyDecomp-binaries_aarch64-Linux.tar.xz";
    sha256 = "sha256-C0tESPXtEN82NhxRoJuC+Jt5CCJlgHMra8FwY9T0NUA=";
  };
  aarch64-darwin = pkgs.callPackage ./tools {
    url = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/v1.2.3/OdysseyDecomp-binaries_arm64-Darwin.tar.xz";
    sha256 = "sha256-vxoPpHNsSLCOOJtqZksrg1qLqronEbhYRt3GZ7+ukI8=";
  };
}
