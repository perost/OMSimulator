#CAPTION#
getStopTime
-----------

Get the stop time from the model.
#END#

#LUA#
.. code-block:: lua

  stopTime, status = oms_getStopTime(cref)

#END#

#PYTHON#
.. code-block:: python

  stopTime, status = oms.getStopTime(cref)

#END#

#CAPI#
.. code-block:: c

  oms_status_enu_t oms_getStopTime(const char* cref, double* stopTime);

#END#

#OMC#
.. code-block:: modelica

  (stopTime, status) := oms_getStopTime(cref);

#END#

#DESCRIPTION#
#END#
