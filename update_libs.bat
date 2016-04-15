@rem This windows bat script needs cygwin and git install to work properly.
@echo Are you sure, this will delete your local modifications to libs? Press Ctrl+C to cancel
@pause
rm -rf rohm-bm1383-glv 
rm -rf rohm-sensor-hal
git clone https://github.com/MikkoZ-Rohm/rohm-sensor-hal1.git rohm-sensor-hal
git clone https://github.com/MikkoZ-Rohm/rohm-bm1383-glv.git
rm -rf rohm-bm1383-glv/.git
rm -rf rohm-sensor-hal/.git

echo Remember to commit the changes if any.
git add rohm-bm1383-glv rohm-sensor-hal
git status