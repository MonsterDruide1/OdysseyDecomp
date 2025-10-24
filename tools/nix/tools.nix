{
  stdenv,
  fetchzip,
  url,
  sha256,
  version,
  autoPatchelfHook,
  zlib,
  openssl,
  ncurses,
  libgcc,
  ...
}:
stdenv.mkDerivation {
  name = "tools";
  inherit version;

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
