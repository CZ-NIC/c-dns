#!/bin/bash
# Push new packaging files to OBS
#
# scripts/prepare_obs.sh script should be run first before this.
# Needs the 'osc' command line tool installed on the system.

set -o errexit -o nounset -o xtrace

pkgdir="pkg/obs"
project=home:CZ-NIC:dns-probe
package=C-DNS

mkdir -p $HOME/.config/osc
echo "[general]" > $HOME/.config/osc/oscrc
echo "apiurl = https://api.opensuse.org" >> $HOME/.config/osc/oscrc
echo "[https://api.opensuse.org]" >> $HOME/.config/osc/oscrc
echo "user=$OBS_USER" >> $HOME/.config/osc/oscrc
echo "pass=$OBS_PASSWORD" >> $HOME/.config/osc/oscrc

# Checkout OBS package
osc co "${project}" "${package}"
pushd "${project}/${package}"

# Delete previous version of the package
osc del * ||:

# Add new version of the package from ${pkgdir}
cp -r ../../${pkgdir}/* ./
osc addremove

# Push changes to OBS
osc ci -n
popd >/dev/null