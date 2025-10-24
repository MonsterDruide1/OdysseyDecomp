pkgs:
let
  toolsJson = (builtins.fromJSON (builtins.readFile ./tools.json));
  version = toolsJson.version;
  cacheUrl = toolsJson.cacheUrlPrefix;
  artifacts = toolsJson.artifacts;
in
builtins.mapAttrs (
  key:
  { hash, filename }:
  pkgs.callPackage ./tools.nix {
    url = "${cacheUrl}${version}/${filename}";
    sha256 = hash;
    inherit version;
  }
) artifacts
