#include "jumptopercentcommand.h"
#include "model/modellocator.h"
JumpToPercentCommand::JumpToPercentCommand(QObject *parent) :
    GitlAbstractCommand(parent)
{
}


bool JumpToPercentCommand::execute( GitlCommandRequest& rcRequest, GitlCommandRespond& rcRespond )
{
    ModelLocator* pModel = ModelLocator::getInstance();
    QVariant vValue = rcRequest.getParameter("percent");
    int iPercent = vValue.toInt();
    int iMaxPOC = pModel->getSequenceManager().getCurrentSequence().getTotalFrames()-1;
    int iMinPOC = 0;
    int iPoc = int(iPercent*iMaxPOC/100.0+0.5);

    /// [Optimization] if poc is not changed, do nothing
    if( iPoc == pModel->getFrameBuffer().getPoc() )
        return true;

    if( iPoc > iMaxPOC || iPoc < iMinPOC )
    {
        return false;
    }

    QPixmap* pcFramePixmap = pModel->getFrameBuffer().getFrame(iPoc);       ///< Read Frame Buffer
    pcFramePixmap = pModel->getDrawEngine().drawFrame(&(pModel->getSequenceManager().getCurrentSequence()), iPoc, pcFramePixmap);  ///< Draw Frame Buffer

    ///
    rcRespond.setParameter("picture",  QVariant::fromValue((void*)(pcFramePixmap)));
    rcRespond.setParameter("current_frame_poc", iPoc);
    rcRespond.setParameter("total_frame_num", pModel->getSequenceManager().getCurrentSequence().getTotalFrames());

    return true;
}
