# MISRA Compliance

The Fleet Provisioning Client Library files conform to the
[MISRA C:2012](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
Deviations from the MISRA standard are listed below:

### Ignored by [Coverity Configuration](tools/coverity/misra.config)
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Directive 4.9 | Advisory | Allow inclusion of function like macros. Asserts, logging, and topic string macros use function like macros. |

### Flagged by Coverity
| Deviation | Category | Justification |
| :-: | :-: | :-: |

### Suppressed with Coverity Comments
*None.*
