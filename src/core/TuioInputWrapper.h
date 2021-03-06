/**
* @file   TuioInputWrapper.h
* @author Tobias Badura <tobias.badura@uni-ulm.de>
* @date   2017.01.31
*
* @brief  Declaration of the TuioInputWrapper.
*/

#pragma once

#ifdef WITH_TUIO
#include <memory>
#include <TuioListener.h>

namespace TUIO {
    class TuioClient;
    class OscReceiver;
}
#else

namespace TUIO {
    using TuioCursor = void;
    using TuioObject = void;
    using TuioBlob = void;
    using TuioTime = int;
}

#endif

namespace viscom::tuio {

#ifdef WITH_TUIO

    class TuioInputWrapper : public TUIO::TuioListener
    {
    public:
        TuioInputWrapper(int port);
        virtual ~TuioInputWrapper() override;

        virtual void addTuioCursor(TUIO::TuioCursor *tcur) override;
        virtual void updateTuioCursor(TUIO::TuioCursor *tcur) override;
        virtual void removeTuioCursor(TUIO::TuioCursor *tcur) override;

        //////////////////////////////////////////////////////////////////////////
        // These methods are currently not used, so do not override them.
        virtual void addTuioObject(TUIO::TuioObject *tobj) override;
        virtual void updateTuioObject(TUIO::TuioObject *tobj) override;
        virtual void removeTuioObject(TUIO::TuioObject *tobj) override;

        virtual void addTuioBlob(TUIO::TuioBlob *tblb) override;
        virtual void updateTuioBlob(TUIO::TuioBlob *tblb) override;
        virtual void removeTuioBlob(TUIO::TuioBlob *tblb) override;

        virtual void refresh(TUIO::TuioTime frameTime) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        std::unique_ptr<TUIO::TuioClient> tuioClient_;
        std::unique_ptr<TUIO::OscReceiver> receiver_;
    };

#else

    class TuioInputWrapper
    {
    public:
        TuioInputWrapper(int port);
        virtual ~TuioInputWrapper();

        virtual void addTuioCursor(TUIO::TuioCursor *tcur) {}
        virtual void updateTuioCursor(TUIO::TuioCursor *tcur) {}
        virtual void removeTuioCursor(TUIO::TuioCursor *tcur) {}

        virtual void addTuioObject(TUIO::TuioObject *tobj) {}
        virtual void updateTuioObject(TUIO::TuioObject *tobj) {}
        virtual void removeTuioObject(TUIO::TuioObject *tobj) {}

        virtual void addTuioBlob(TUIO::TuioBlob *tblb) {}
        virtual void updateTuioBlob(TUIO::TuioBlob *tblb) {}
        virtual void removeTuioBlob(TUIO::TuioBlob *tblb) {}

        virtual void refresh(TUIO::TuioTime frameTime) {}
    };

#endif
}
