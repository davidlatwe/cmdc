#define _doc_TransformationMatrix_asMatrix \
    "Interpolates between the identity transformation and that currently in the object, returning the result as an MMatrix."

#define _doc_TransformationMatrix_asMatrixInverse \
    "Returns the inverse of the matrix representing the transformation."

#define _doc_TransformationMatrix_asScaleMatrix \
    "Returns the matrix which takes points from object space to the space immediately following scale and shear transformations."

#define _doc_TransformationMatrix_asRotateMatrix \
    "Returns the matrix which takes points from object space to the space immediately following the scale/shear/rotation transformations."

#define _doc_TransformationMatrix_setScale \
    "Sets the transformation's scale components to the three floats in the provided sequence."

#define _doc_TransformationMatrix_setRotation \
    "Sets the transformation's rotation component."

#define _doc_TransformationMatrix_rotationOrder \
    "Returns the order of rotations when the transformation's rotate component is expressed as an euler rotation."

#define _doc_TransformationMatrix_reorderRotation \
    "Reorders the transformation's rotate component to give the same overall rotation but using a new order or rotations."

#define _doc_TransformationMatrix_setToRotationAxis \
    "Sets the transformation's rotate component to be a given axis vector and angle in radians."

#define _doc_TransformationMatrix_rotationOrientation \
    "Returns a quaternion which orients the local rotation space."

#define _doc_TransformationMatrix_setRotationOrientation \
    "Sets a quaternion which orients the local rotation space."

#define _doc_TransformationMatrix_setTranslation \
    "Sets the transformation's translation component."

#define _doc_TransformationMatrix_setShear \
    "Sets the transformation's shear component."

#define _doc_TransformationMatrix_scalePivot \
    "Returns the transformation's scale pivot component."

#define _doc_TransformationMatrix_setScalePivot \
    "Sets the transformation's scale pivot component."

#define _doc_TransformationMatrix_scalePivotTranslation \
    "Returns the transformation's scale pivot translation component."

#define _doc_TransformationMatrix_setScalePivotTranslation \
    "Sets the transformation's scale pivot translation component."

#define _doc_TransformationMatrix_rotatePivot \
    "Returns the transformation's rotate pivot component."

#define _doc_TransformationMatrix_setRotatePivot \
    "Sets the transformation's rotate pivot component."

#define _doc_TransformationMatrix_rotatePivotTranslation \
    "Returns the transformation's rotate pivot translation component."

#define _doc_TransformationMatrix_setRotatePivotTranslation \
    "Sets the transformation's rotate pivot translation component."

#define _doc_TransformationMatrix_isEquivalent \
    "Returns true if this transformation's matrix is within tolerance of another's matrix."

py::enum_<MTransformationMatrix::RotationOrder>(TransformationMatrix, "RotationOrder")
    .value("kInvalid", MTransformationMatrix::RotationOrder::kInvalid) 
    .value("kXYZ", MTransformationMatrix::RotationOrder::kXYZ)
    .value("kYZX", MTransformationMatrix::RotationOrder::kYZX)
    .value("kZXY", MTransformationMatrix::RotationOrder::kZXY)
    .value("kXZY", MTransformationMatrix::RotationOrder::kXZY)
    .value("kYXZ", MTransformationMatrix::RotationOrder::kYXZ)
    .value("kZYX", MTransformationMatrix::RotationOrder::kZYX)
    .value("kLast", MTransformationMatrix::RotationOrder::kLast)
    .export_values();

TransformationMatrix
    .def(py::init<>())
    .def(py::init<const MMatrix &>(), py::arg("src"))
    .def(py::init<const MTransformationMatrix &>(), py::arg("src"))

    .def("asMatrix", [](MTransformationMatrix & self, double interp = 1.0) -> MMatrix {
        return self.asMatrix(interp);
    }, py::arg("interp") = 1.0, _doc_TransformationMatrix_asMatrix)

    .def("asMatrixInverse", &MTransformationMatrix::asMatrixInverse, _doc_TransformationMatrix_asMatrixInverse)

    .def("asRotateMatrix", &MTransformationMatrix::asRotateMatrix, _doc_TransformationMatrix_asRotateMatrix)

    .def("asScaleMatrix", &MTransformationMatrix::asScaleMatrix, _doc_TransformationMatrix_asScaleMatrix)

    .def("isEquivalent", &MTransformationMatrix::isEquivalent, py::arg("other"), py::arg("tolerance") = MTransformationMatrix_kTol, _doc_TransformationMatrix_isEquivalent)

    .def("reorderRotation", [](MTransformationMatrix & self, MTransformationMatrix::RotationOrder order) {
        MStatus status = self.reorderRotation(order);
        
        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("order"), _doc_TransformationMatrix_reorderRotation)

    .def("rotatePivot", [](MTransformationMatrix & self, MSpace::Space space) -> MPoint {
        MStatus status;
        MPoint result = self.rotatePivot(space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }

        return result;
    }, py::arg("space"), _doc_TransformationMatrix_rotatePivot)

    .def("rotatePivotTranslation", [](MTransformationMatrix & self, MSpace::Space space) -> MVector {
        MStatus status;
        MVector result = self.rotatePivotTranslation(space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }

        return result;
    }, py::arg("space"), _doc_TransformationMatrix_rotatePivotTranslation)

    .def_property_readonly("quaternionRotation", &MTransformationMatrix::rotation,
        "Returns the rotation component of the transformation matrix as a quaternion.")

    .def_property_readonly("eulerRotation", &MTransformationMatrix::eulerRotation,
        "Returns the rotation component of the transformation matrix as an euler rotation.")
    
    .def("setRotation", [](MTransformationMatrix & self, const MQuaternion& rotation) {
        self.rotateTo(rotation);
    }, py::arg("rotation"), "Set the rotation component of the transformation matrix using a quaternion.")

    .def("setRotation", [](MTransformationMatrix & self, const MEulerRotation& rotation) {
        self.rotateTo(rotation);
    }, py::arg("rotation"), "Set the rotation component of the transformation matrix using an euler rotation.")

    .def("rotateBy", [](MTransformationMatrix & self, const MQuaternion& rotate, MSpace::Space space) {
        MStatus status;
        self.rotateBy(rotate, space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("rotate"), py::arg("space"), "Adds to the rotation component of the rotation matrix by rotating relative to the existing transformation using a quaternion.")

    .def("rotateBy", [](MTransformationMatrix & self, const MEulerRotation& rotate, MSpace::Space space) {
        MStatus status;
        self.rotateBy(rotate, space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("rotate"), py::arg("space"), "Adds to the rotation component of the rotation matrix by rotating relative to the existing transformation using an euler rotation.")

    .def("getRotationQuaternion", [](MTransformationMatrix & self) -> MQuaternion {
        double x, y, z, w;
        const MStatus status = self.getRotationQuaternion(x, y, z, w);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
        return MQuaternion(x, y, z, w);
    }, "Get the rotation component of the transformation matrix as a quaternion. (in MSpace::kTransform space)")

    .def("setRotationQuaternion", [](MTransformationMatrix & self, double x, double y, double z, double w) {
        const MStatus status = self.setRotationQuaternion(x, y, z, w);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"), "Set the rotation component of the transformation matrix using a quaternion. (in MSpace::kTransform space)")

    .def("addRotationQuaternion", [](MTransformationMatrix & self, double x, double y, double z, double w, MSpace::Space space) {
        const MStatus status = self.addRotationQuaternion(x, y, z, w, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"), py::arg("space"),
    "Add to the rotation component by rotating relative to the existing transformation.\n"
    "The only valid transformation spaces for this method are MSpace::kTransform and MSpace::kPreTransform. All other spaces are treated as being equivalent to MSpace::kTransform.")

    .def_property_readonly("rotationOrder", [](MTransformationMatrix & self) -> MTransformationMatrix::RotationOrder {
        MStatus status;
        auto result = self.rotationOrder(&status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
        
        return result;
    }, _doc_TransformationMatrix_rotationOrder)

    .def("rotationOrientation", &MTransformationMatrix::rotationOrientation, _doc_TransformationMatrix_rotationOrientation)

    .def("setRotationOrientation", &MTransformationMatrix::setRotationOrientation, py::arg("q"), _doc_TransformationMatrix_setRotationOrientation)

    .def("scalePivot", [](MTransformationMatrix & self, MSpace::Space space) -> MPoint {
        MStatus status;
        MPoint result = self.scalePivot(space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }

        return result;
    }, py::arg("space"), _doc_TransformationMatrix_scalePivot)

    .def("scalePivotTranslation", [](MTransformationMatrix & self, MSpace::Space space) -> MVector {
        MStatus status;
        MVector result = self.scalePivotTranslation(space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }

        return result;
    }, py::arg("space"), _doc_TransformationMatrix_scalePivotTranslation)

    .def("setRotatePivot", [](MTransformationMatrix & self, MPoint point, MSpace::Space space, bool balance) {
        MStatus status = self.setRotatePivot(point, space, balance);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("point"), py::arg("space"), py::arg("balance"), _doc_TransformationMatrix_setRotatePivot)

    .def("setRotatePivotTranslation", [](MTransformationMatrix & self, MVector vector, MSpace::Space space) {
        MStatus status = self.setRotatePivotTranslation(vector, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("vector"), py::arg("space"), _doc_TransformationMatrix_setRotatePivotTranslation)

    .def("scale", [](MTransformationMatrix & self, MSpace::Space space) -> py::list {
        py::list result;
        double scale[3];
        MStatus status = self.getScale(scale, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }

        result.append(scale[0]);
        result.append(scale[1]);
        result.append(scale[2]);
        
        return result;
    }, py::arg("space"), "Get the scale component of the transformation matrix and retun it as a list of 3 floats.")

    .def("setScale", [](MTransformationMatrix & self, const py::list & scale, MSpace::Space space) {
        if (scale.size() != 3)
        {
            throw std::invalid_argument("You must provide a list of 3 floats for scale.");
        }
        double tmp[3];
        std::transform(std::begin(scale), std::end(scale), std::begin(tmp),
                [](pybind11::handle handle) -> double { return handle.cast<double>(); });

        MStatus status = self.setScale(tmp, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("scale"), py::arg("space"), _doc_TransformationMatrix_setScale)

    .def("scaleBy", [](MTransformationMatrix & self, const py::list & scale, MSpace::Space space) {
        if (scale.size() != 3)
        {
            throw std::invalid_argument("You must provide a list of 3 floats for scale.");
        }
        double tmp[3];
        std::transform(std::begin(scale), std::end(scale), std::begin(tmp),
                [](pybind11::handle handle) -> double { return handle.cast<double>(); });

        MStatus status = self.addScale(tmp, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("scale"), py::arg("space"), "Multiplies the transformation's scale components by the three floats in the provided sequence.")

    .def("setScalePivot", [](MTransformationMatrix & self, MPoint point, MSpace::Space space, bool balance) {
        MStatus status = self.setScalePivot(point, space, balance);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("point"), py::arg("space"), py::arg("balance"), _doc_TransformationMatrix_setScalePivot)

    .def("setScalePivotTranslation", [](MTransformationMatrix & self, MVector vector, MSpace::Space space) {
        MStatus status = self.setScalePivotTranslation(vector, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("vector"), py::arg("space"), _doc_TransformationMatrix_setScalePivotTranslation)

    .def("setShear", [](MTransformationMatrix & self, const py::list & shear, MSpace::Space space) {
        if (shear.size() != 3)
        {
            throw std::invalid_argument("You must provide a list of 3 floats for shear.");
        }
        double tmp[3];
        std::transform(std::begin(shear), std::end(shear), std::begin(tmp),
                [](pybind11::handle handle) -> double { return handle.cast<double>(); });

        MStatus status = self.setShear(tmp, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("shear"), py::arg("space"), _doc_TransformationMatrix_setShear)

    .def("setToRotationAxis", [](MTransformationMatrix & self, MVector axis, double rotation) {
        MStatus status = self.setToRotationAxis(axis, rotation);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("axis"), py::arg("rotation"), _doc_TransformationMatrix_setToRotationAxis)

    .def("getTranslation", [](MTransformationMatrix & self, MSpace::Space space) -> MVector {
        MStatus status;
        MVector translation = self.getTranslation(space, &status);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
        return translation;
    }, py::arg("space"), "Returns the translation component of the translation as a vector in centimeters.")

    .def("setTranslation", [](MTransformationMatrix & self, MVector vector, MSpace::Space space) {
        MStatus status = self.setTranslation(vector, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("vector"), py::arg("space"), _doc_TransformationMatrix_setTranslation)

    .def("addTranslation", [](MTransformationMatrix & self, MVector vector, MSpace::Space space) {
        MStatus status = self.addTranslation(vector, space);

        if (!status) {
            throw std::logic_error(status.errorString().asChar());
        }
    }, py::arg("vector"), py::arg("space"), "Add to the translation component by translating relative to the existing transformation.")

;

// Alias
TransformationMatrix.attr("translation") = TransformationMatrix.attr("getTranslation");
TransformationMatrix.attr("translateBy") = TransformationMatrix.attr("addTranslation");
TransformationMatrix.attr("rotateOrientation") = TransformationMatrix.attr("rotationOrientation");  // Maya 2018
TransformationMatrix.attr("setRotateOrientation") = TransformationMatrix.attr("setRotationOrientation");    // Maya 2018
