#include "RecoVertex/VertexTools/src/TwoTrackMinimumDistanceLineLine.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "Geometry/Vector/interface/GlobalVector.h"
#include "Geometry/Vector/interface/GlobalPoint.h"


bool TwoTrackMinimumDistanceLineLine::calculate(
    const GlobalTrajectoryParameters & theG,
    const GlobalTrajectoryParameters & theH)
{
  if ( theH.charge() != 0. || theG.charge() != 0. )
  {
    cout << "[TwoTrackMinimumDistanceImplementation] Error: "
         << "charge of input track is not zero." << endl;
    return true;
  };

  GlobalVector gVec = theG.momentum();
  const double gMag= theG.momentum().mag(); double gMag2 = gMag*gMag;
  GlobalVector hVec = theH.momentum();
  const double hMag= theH.momentum().mag(); double hMag2 = hMag*hMag;

  if ( gMag == 0. || hMag == 0. )
  {
    cout << "[TwoTrackMinimumDistanceImplementation] Error: "
         << "momentum of input trajectory is zero." << endl;
    return true;
  };

  phiG = theG.momentum().phi();
  phiH = theH.momentum().phi();

  double gVec_Dot_hVec = gVec.dot(hVec);
  double norm  = gVec_Dot_hVec*gVec_Dot_hVec - gMag2*hMag2;

  if ( norm == 0 )
  {
    cout << "[TwoTrackMinimumDistanceImplementation] Error: "
         << "Tracks are parallel." << endl;
    return true;
  }

  GlobalPoint gOrig = theG.position();
  GlobalPoint hOrig = theH.position();
  GlobalVector posDiff = gOrig - hOrig;

  double tG = (posDiff.dot(gVec) * hMag2 - gVec_Dot_hVec * posDiff.dot(hVec))/ norm;
  double tH = (gVec_Dot_hVec * posDiff.dot(gVec) - posDiff.dot(hVec) * gMag2)/ norm;

  gPos = GlobalPoint ( gOrig + tG * gVec);
  hPos = GlobalPoint ( hOrig + tH * hVec);
//   cout << "TwoTrackMinimumDistanceLineLine "<<gPos<<hPos<<endl;
  return false;
}

pair <GlobalPoint, GlobalPoint> TwoTrackMinimumDistanceLineLine::points() const
{
  return pair <GlobalPoint, GlobalPoint > ( gPos, hPos );
}
