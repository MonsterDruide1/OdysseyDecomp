pkgs:
let
  artifacts = (builtins.fromJSON (builtins.readFile ./artifacts.json));
  cache = (builtins.fromJSON (builtins.readFile ../cache-version.json));
  urlPrefix = cache.urlPrefix;
  version = cache.version;
in
{
  cacheUrl = "${urlPrefix}/${version}";
  artifacts = builtins.mapAttrs (
    key:
    { hash, filename }:
    pkgs.callPackage ./tools.nix {
      url = "${urlPrefix}${version}/${filename}";
      sha256 = hash;
      inherit version;
    }
  ) artifacts;
}
