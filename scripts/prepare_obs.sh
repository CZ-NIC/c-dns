#!/bin/bash
# Create source packaging files for OBS (OpenSUSE Build Service)
#
# Needs system with following software:
# - dpkg-buildpackage
# - apkg

set -o errexit -o nounset -o xtrace

pushd "$(dirname ${0})/.."

OUTDIR="pkg/obs"

# Clean existing output files
rm -rf "$OUTDIR"

# Generate debian source package and remove extra source package files
apkg srcpkg -O $OUTDIR -d debian
rm -f $OUTDIR/*_source.*

# Generate RPM SPEC file template and modify it to use debian source archive
apkg srcpkg -O $OUTDIR -d fedora --render-template
sed -i 's/^Source:.*/Source: c-dns_%{version}.orig.tar.gz/' $OUTDIR/*.spec

# Generate PKGBUILD template and modify it to use debian source archive
apkg srcpkg -O $OUTDIR -d arch --render-template
sed -i 's/^source=.*/source=("c-dns_${pkgver}.orig.tar.gz")/' $OUTDIR/PKGBUILD

popd >/dev/null
