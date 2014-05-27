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

s=neo.loadWaveFromFile('/home/macrobull/workspace/kdev/ED/test/RAW051.BIN',Sa=162.7e3,ch1Div=1,ch2Div=1)
s1=neo.loadWaveFromFile('/home/macrobull/workspace/kdev/ED/test/RAW052.BIN',Sa=162.7e3,ch1Div=1,ch2Div=1)

#s.ch2 = s1.ch1
s.chCnt+=1;
s.chs.append(s1.ch2)
s.chTitles = [u"LED波形", u"暗", u"明"]
s.chUnits.append("V")
s.timeOrder -= 3

s.ch1 = avg(5e-5, s.t, s.ch1)
s.ch1 -= s.ch1.min()

s.ch2 = avg(5e-5, s.t, s.ch2)
s.ch2 +=2

s.chs[2] = avg(5e-5, s.t, s.chs[2])
s.chs[2] +=2


subplot(111, title = u"不同明暗环境下信号放大后波形")

s.plotWave(**defaultPlotArgs)

ylim(-0.5, 3.5)
xlim(-3, 3)

show()
