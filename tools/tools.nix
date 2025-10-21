{
  stdenv,
  fetchzip,
  url,
  sha256,
  autoPatchelfHook,
  zlib,
  openssl,
  ncurses,
  libgcc,
  ...
}:
stdenv.mkDerivation {
  name = "tools";
  version = "1.2.3";

  src = fetchzip {
    inherit url sha256;
  };

  nativeBuildInputs = [
    autoPatchelfHook
    zlib
    openssl
    ncurses
    libgcc.lib
  ];

  installPhase = ''
    mkdir $out/bin -p
    cp -r $src/* $out/bin
  '';
}
