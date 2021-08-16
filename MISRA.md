# MISRA Compliance

The Fleet Provisioning Library files conform to the [MISRA
C:2012](https://www.misra.org.uk) guidelines, with some noted exceptions.
Compliance is checked with Coverity static analysis. Deviations from the MISRA
standard are listed below:

### Ignored by [Coverity Configuration](tools/coverity/misra.config)
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Directive 4.9 | Advisory | Allow inclusion of function like macros. Asserts, logging, and topic string macros use function like macros. |
| Rule 2.5 | Advisory | Allow unused macros. Macros defined for topic strings are not used by the library, but are part of the API. |
| Rule 3.1 | Required | Allow nested comments. C++ style `//` comments are used in example code within Doxygen documentation blocks. |

### Flagged by Coverity
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Rule 8.7 | Advisory | API functions are not used by the library outside of the files they are defined; however, they must be externally visible in order to be used by an application. |

### Suppressed with Coverity Comments
*None.*
