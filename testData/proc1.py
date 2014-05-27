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

s=neo.loadWaveFromFile('/home/macrobull/workspace/kdev/ED/test/RAW066.BIN',Sa=2.543e3,ch1Div=1,ch2Div=1)

s.timeOrder -= 3

s.ch1 = avg(1e-1, s.t, s.ch1)

s.chCnt = 1


subplot(111, title = u"晶振敲击脉冲")

s.plotWave(**defaultPlotArgs)


show()
