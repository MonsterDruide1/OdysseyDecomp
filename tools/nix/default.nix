pkgs:
let
  artifactDefs = (builtins.fromJSON (builtins.readFile ./artifacts.json));
  cache = (builtins.fromJSON (builtins.readFile ../cache-version.json));
  urlPrefix = cache.urlPrefix;
  version = cache.version;
in
rec {
  cacheUrl = "${urlPrefix}/${version}";
  artifacts = builtins.mapAttrs (
    key:
    { filename, hash }:
    pkgs.callPackage ./tools.nix {
      url = "${cacheUrl}/${filename}";
      sha256 = hash;
      inherit version;
    }
  ) artifactDefs;
}
