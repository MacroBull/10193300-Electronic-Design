# -*- coding: utf-8 -*-
"""
Project	:Python-Project
Version	:0.0.1
@author	:MacroBull
"""

#from macrobull.oscProcess import *
import macrobull.oscProcess.neoScope as neo
from macrobull.oscProcess.oscUtility import *
from macrobull.misc import average as avg, defaultPlotArgs, extend, tex

s=neo.loadWaveFromFile('/home/macrobull/workspace/kdev/ED/test/RAW054.BIN',Sa=162.7e3,ch1Div=1,ch2Div=1)
s1=neo.loadWaveFromFile('/home/macrobull/workspace/kdev/ED/test/RAW055.BIN',Sa=162.7e3,ch1Div=1,ch2Div=1)

s.ch1 = s.ch2
s.ch2 = s1.ch2

s.timeOrder -= 3

s.ch1 = avg(5e-5, s.t, s.ch1)
s.ch1 +=2

s.ch2 = avg(5e-5, s.t, s.ch2)
s.ch2 +=2



subplot(111, title = u"手指距离-检波后波形")

s.plotWave(**defaultPlotArgs)

ylim(-0.5, 3.5)
xlim(-3, 3)

show()
