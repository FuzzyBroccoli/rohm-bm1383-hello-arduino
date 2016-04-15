@rem This windows bat script needs cygwin and git install to work properly.
mkdir publish
cd publish
git clone -n https://github.com/MikkoZ-Rohm/rohm-sensor-hal1.git
git clone -n https://github.com/MikkoZ-Rohm/rohm-bm1383-glv.git
mv rohm-sensor-hal1/.git ../rohm-sensor-hal/
mv rohm-bm1383-glv/.git ../rohm-bm1383-glv/
cd ..
rm -rf publish

cd rohm-bm1383-glv
git add .
git status
cd ..

cd rohm-sensor-hal/
git add .
git status 
cd ..

echo Remember to commit under library directories to upstream the changes.
