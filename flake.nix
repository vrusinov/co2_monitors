# SPDX-FileCopyrightText: 2024 Vladimir Rusinov
# SPDX-License-Identifier: Apache-2.0
{
  description = "CO2 Monitors - ESPHome project with development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # ESPHome and dependencies
            esphome
            python312
            python312Packages.pip

            # Pre-commit hooks tools
            pre-commit
            clang-tools
            nodePackages.markdownlint-cli
            nodePackages.yamllint
            reuse
            typos
            cpplint
            ruby
            bundler
            openssl
            zlib
            libffi
            readline
            gcc
            pkg-config

            # Utilities
            git
            gnumake
          ];

          shellHook = ''
            export LDFLAGS="-L${pkgs.openssl}/lib -L${pkgs.zlib}/lib -L${pkgs.libffi}/lib -L${pkgs.readline}/lib"
            export CPPFLAGS="-I${pkgs.openssl}/include -I${pkgs.zlib}/include -I${pkgs.libffi}/include -I${pkgs.readline}/include"
            export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [pkgs.openssl pkgs.zlib pkgs.libffi pkgs.readline]}:$LD_LIBRARY_PATH"
          '';
        };
      }
    );
}
